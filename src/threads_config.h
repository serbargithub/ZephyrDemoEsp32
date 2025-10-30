#ifndef H_THREADS_CONFIG_H_
#define H_THREADS_H_

#include <zephyr/kernel.h>
#include "uart_console_thread.h"
//#include "led_driver.h"


/* --- Threads configuration --- */
#define UART_STACK_SIZE   1024
#define UART_PRIORITY     5

#define LED_STACK_SIZE    512
#define LED_PRIORITY      10


/* --- Threads definitions --- */
K_THREAD_DEFINE(uart_console_thread_id, UART_STACK_SIZE,
                uart_console_thread_fn, NULL, NULL, NULL,
                UART_PRIORITY, 0, 0);


#endif /* H_THREADS_CONFIG_H_ */
