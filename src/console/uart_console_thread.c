#include <zephyr/kernel.h>
#include "uart_console.h"

#include "../log/logger.h"
LOG_MODULE_REGISTER(uart_console_thread);


void uart_console_thread_fn(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);

    if (uart_console_start() != 0) {
        LOG_ERR("UART console init failed");
        return;
    }

    while (1) {
        uart_console_task_handler();
    }
}