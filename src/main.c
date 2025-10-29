#include "uart_console.h"

#include "log/logger.h"
LOG_MODULE_REGISTER(main, LOG_LEVEL_DEFAULT);

int main(void)
{

    LOG_INF("ESP32 Demo");
    uart_console_start();

    while (1)
    {
        uart_console_task_handler();
        k_sleep(K_MSEC(10));
    }


    return 0;
}
