
#ifndef H_UART_CONSOLE_
#define H_UART_CONSOLE_

#include <stdint.h>

int uart_console_start(void);
void uart_console_task_handler(void);
void uart_console_stop(void);


#endif // H_UART_CONSOLE_

