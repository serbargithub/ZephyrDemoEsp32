#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/console/tty.h>
#include "cli_handler.h"    
#include "uart_console.h"

#include "../log/logger.h"
LOG_MODULE_REGISTER(uart_console, LOG_LEVEL_DEFAULT);

#define UART_NODE DT_NODELABEL(uart0)
static const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);

/* buffers */
#define RX_BUF_SIZE 256
#define TX_BUF_SIZE 128
#define LINE_BUF_SIZE 128
#define TX_RX_TIMEOUT_MS 10

static uint8_t rx_buf[RX_BUF_SIZE];
static uint8_t tx_buf[TX_BUF_SIZE];
static  char linebuf[LINE_BUF_SIZE];
static struct tty_serial tty;
static  size_t line_pos = 0;


void write_handler(char *data, uint32_t length)
{
    tty_write(&tty, data, length);
}


static void process_line(char *line)
{
    size_t length = strlen(line);
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

void uart_console_task_handler(void)
{

    uint8_t ch;
    ssize_t r = tty_read(&tty, &ch, 1); 

    if (r > 0)
    {
        tty_write(&tty, &ch, 1); /* echo back */

        if ((ch == '\r') || (ch == '\n'))
        {
            char *new_line = "\r\n";
            tty_write(&tty, new_line, sizeof(new_line));
            linebuf[line_pos < LINE_BUF_SIZE ? line_pos : (LINE_BUF_SIZE - 1)] = '\0';
            process_line(linebuf);
            line_pos = 0;
        }
        else
        {
            if (line_pos < (LINE_BUF_SIZE - 1))
            {
                linebuf[line_pos++] = (char)ch;
            }
            else
            {
                /* overflow: terminate and process, then reset */
                linebuf[LINE_BUF_SIZE - 1] = '\0';
                process_line(linebuf);
                line_pos = 0;
            }
        }
    }
    else
    {
        k_sleep(K_MSEC(10));
    }
}


void uart_console_stop(void){

    LOG_INF("Stopping UART console");
}

