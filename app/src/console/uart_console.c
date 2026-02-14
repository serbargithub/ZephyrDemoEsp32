#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/console/tty.h>
#include "cli_handler.h"    
#include "uart_console.h"

#include "../log/logger.h"
LOG_MODULE_REGISTER(uart_console);

#define UART_NODE DT_NODELABEL(uart0)
static const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);

/* buffers */
#define RX_BUF_SIZE 256
#define TX_BUF_SIZE 128
#define LINE_BUF_SIZE 128
#define TX_RX_TIMEOUT_MS 10
#define LINE_STACK_SIZE 5

static uint8_t rx_buf[RX_BUF_SIZE];
static uint8_t tx_buf[TX_BUF_SIZE];
static  char linebuf[LINE_BUF_SIZE];
static struct tty_serial tty;
static  size_t line_pos = 0;

/* command history */
static char line_history[LINE_STACK_SIZE][LINE_BUF_SIZE];
static int history_count = 0;  
static int history_index = -1;

static bool esc_seq = false;
static char esc_buf[3];
static int esc_pos = 0;


static void write_handler(char *data, uint32_t length)
{
    tty_write(&tty, data, length);
}

static void add_to_history(const char *line)
{
    if (strlen(line) == 0) return;

    /* shift if full */
    if (history_count == LINE_STACK_SIZE) {
        for (int i = 1; i < LINE_STACK_SIZE; i++) {
            strcpy(line_history[i - 1], line_history[i]);
        }
        history_count--;
    }
    strcpy(line_history[history_count++], line);
}

static void show_line(const char *line)
{
    const char *clear_seq = "\r\033[K"; // CR + clear to end of line
    tty_write(&tty, clear_seq, strlen(clear_seq));
    tty_write(&tty, line, strlen(line));
}

static void process_line(char *line)
{
    size_t length = strlen(line);
    add_to_history(line);
    history_index = -1;
    cli_handler_process_command(line, length, &write_handler);
}

int uart_console_start(void)
{
    if (!uart_dev)
    {
        LOG_ERR("Console UART not found");
        return 1;
    }

    if (tty_init(&tty, uart_dev) < 0)
    {
        LOG_ERR("tty_init failed");
        return 1;
    }

    tty_set_rx_buf(&tty, rx_buf, sizeof(rx_buf));
    tty_set_tx_buf(&tty, tx_buf, sizeof(tx_buf));

    tty.rx_timeout = TX_RX_TIMEOUT_MS; 
    tty.tx_timeout = TX_RX_TIMEOUT_MS;

    line_pos = 0;

    LOG_INF("Non-blocking tty line reader started");
    return 0;
}

static void handle_arrow_up(void)
{
    if (history_count == 0) return;

    if (history_index < history_count - 1) {
        history_index++;
    }

    strcpy(linebuf, line_history[history_count - 1 - history_index]);
    line_pos = strlen(linebuf);

    show_line(linebuf);
}

static void handle_arrow_down(void)
{
    if (history_count == 0)
        return;

    if (history_index > 0) {
        history_index--;
        strcpy(linebuf, line_history[history_count - 1 - history_index]);
    } else {
        /* If already at newest command, clear the input line */
        history_index = -1;
        linebuf[0] = '\0';
    }

    line_pos = strlen(linebuf);
    show_line(linebuf);
}

static void handle_char(uint8_t ch)
{
    if (esc_seq) {
        esc_buf[esc_pos++] = ch;
        if (esc_pos == 2) {
            if (esc_buf[0] == '[') {
                switch (esc_buf[1]) {
                    case 'A': handle_arrow_up(); break;   /* ↑ */
                    case 'B': handle_arrow_down(); break; /* ↓ */
                    default: break;
                }
            }
            esc_seq = false;
            esc_pos = 0;
        }
        return;
    }

    if (ch == 0x1B) {  /* ESC starts an escape sequence */
        esc_seq = true;
        esc_pos = 0;
        return;
    }

    if (ch == '\r' || ch == '\n') {
        const char *newline = "\r\n";
        tty_write(&tty, newline, strlen(newline));
        linebuf[line_pos < LINE_BUF_SIZE ? line_pos : (LINE_BUF_SIZE - 1)] = '\0';
        process_line(linebuf);
        line_pos = 0;
        linebuf[0] = '\0';
    } else if (ch == 0x08 || ch == 0x7F) { /* Backspace or DEL */
        if (line_pos > 0) {
            line_pos--;
            linebuf[line_pos] = '\0';
            const char *back = "\b \b";
            tty_write(&tty, back, strlen(back));
        }
    } else {
        if (line_pos < LINE_BUF_SIZE - 1) {
            linebuf[line_pos++] = (char)ch;
            tty_write(&tty, &ch, 1); /* echo */
        }
    }
}

void uart_console_task_handler(void)
{
    uint8_t ch;
    ssize_t r = tty_read(&tty, &ch, 1);

    if (r > 0) {
        handle_char(ch);
    } else {
        k_sleep(K_MSEC(10));
    }
}

