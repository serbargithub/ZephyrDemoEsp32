#include "uart_console.h"

#include "log/logger.h"
LOG_MODULE_REGISTER(main, LOG_LEVEL_DEFAULT);

int main(void)
{

    int ret = uart_console_start();
    if (ret != 0) {
        LOG_ERR("Failed to start UART console: %d", ret);
        return ret;
    }

    while (1)
    {
        uart_console_task_handler();
        k_sleep(K_MSEC(10));
    }


    return 0;
}
