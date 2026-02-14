#include "drivers/led_color.h"
#include "log/logger.h"
#include "threads_config.h"
#include <zephyr/kernel.h>
LOG_MODULE_REGISTER(main);

int main(void)
{
    LOG_INF("ESP32 demo started");

    while (1)
    {
        // Blink is device alive indicator
        if (led_color_is_blink_allowed() == false)
        {
            k_sleep(K_SECONDS(5));
            led_color_allow_blink();
        }
        else
        {
            led_color_blink_indicator(LED_BLINK_ON);
            k_sleep(K_MSEC(500));
            led_color_blink_indicator(LED_BLINK_OFF);
            k_sleep(K_MSEC(500));
        }
    }

    return 0;
}
