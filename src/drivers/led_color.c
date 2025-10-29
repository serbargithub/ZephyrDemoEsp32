#include "led_color.h"
#include <zephyr/drivers/led_strip.h>
#include "../log/logger.h"
LOG_MODULE_REGISTER(led_color, LOG_LEVEL_DEFAULT);

#define STRIP_NODE DT_ALIAS(led_strip)
#define CONFIG_LED_BRIGHTNESS 64

static struct led_rgb pixel;
static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

#define RGB(_r, _g, _b) {.r = (_r), .g = (_g), .b = (_b)}

static const struct led_rgb colors[] = {
	RGB(CONFIG_LED_BRIGHTNESS, 0x00, 0x00), /* red */
	RGB(0x00, CONFIG_LED_BRIGHTNESS, 0x00), /* green */
	RGB(0x00, 0x00, CONFIG_LED_BRIGHTNESS), /* blue */
	RGB(0x00, 0x00, 0x00),					/* off */
};

static bool is_led_strip_ready(void)
{
	if (device_is_ready(strip))
	{
		return true;
	}
	else
	{
		LOG_ERR("LED strip device %s is not ready", strip->name);
		return false;
	}
}

int led_color_set_color(enum led_color color)
{
	if (is_led_strip_ready() == false)
	{
		return 0;
	}
	pixel = colors[color];
	int rc = led_strip_update_rgb(strip, &pixel, 1);
	if (rc)
	{
		LOG_ERR("couldn't update strip: %d", rc);
		return 0;
	}
	return rc;
}

int led_color_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	if (is_led_strip_ready() == false)
	{
		return 0;
	}
	pixel.r = r;
	pixel.g = g;
	pixel.b = b;
	int rc = led_strip_update_rgb(strip, &pixel, 1);
	if (rc)
	{
		LOG_ERR("couldn't update strip: %d", rc);
		return 0;
	}
	return rc;
}