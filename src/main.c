#include "led_color.h"
#include "logger.h"
LOG_MODULE_REGISTER(main, LOG_LEVEL_DEFAULT);

//#include <zephyr/kernel.h>
//#include <zephyr/device.h>
//#include <zephyr/drivers/spi.h>
//#include <zephyr/sys/util.h>


#define DELAY_TIME K_MSEC(200)

int main(void)
{


	LOG_INF("Displaying led colors in a loop");
	while (1) {
		for (enum led_color current_color = 0; current_color < sizeof(enum led_color); current_color++) {
			int rc = led_color_set_color(current_color);
			if (rc < 0) {
				LOG_ERR("couldn't set color %d: %d", current_color, rc);
				return rc;
			}

			k_sleep(DELAY_TIME);
		}

	}

	return 0;
}
