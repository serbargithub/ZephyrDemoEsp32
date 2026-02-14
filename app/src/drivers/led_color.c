#include <zephyr/kernel.h>
#include "led_color.h"
#include <zephyr/drivers/led_strip.h>

#include "../log/logger.h"
LOG_MODULE_REGISTER(led_color);

#define STRIP_NODE DT_ALIAS(led_strip)
#define CONFIG_LED_BRIGHTNESS 64

static struct led_rgb pixel;
static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

//static struct k_mutex led_rgb_mutex;
K_MUTEX_DEFINE(led_rgb_mutex);
static volatile bool is_blink_indicator_allowed = false;

#define RGB(_r, _g, _b) {.r = (_r), .g = (_g), .b = (_b)}

static const struct led_rgb colors[] = {
	RGB(CONFIG_LED_BRIGHTNESS, 0x00, 0x00), /* red */
	RGB(0x00, CONFIG_LED_BRIGHTNESS, 0x00), /* green */
	RGB(0x00, 0x00, CONFIG_LED_BRIGHTNESS), /* blue */
	RGB(0x00, 0x00, 0x00),					/* off */
};


int led_color_set_color(enum led_color color)
{
	if (device_is_ready(strip) == false)
	{
		LOG_ERR("Led strip not ready");
		return -ENODEV;
	}
	k_mutex_lock(&led_rgb_mutex, K_FOREVER);
	pixel = colors[color];
	int rc = led_strip_update_rgb(strip, &pixel, 1);
	if (rc)
	{
		LOG_ERR("couldn't update strip: %d", rc);
	}
	is_blink_indicator_allowed = false;
	k_mutex_unlock(&led_rgb_mutex);
	return rc;
}

int led_color_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	if (device_is_ready(strip) == false)
	{
		LOG_ERR("Led strip not ready");
		return -ENODEV;
	}
	k_mutex_lock(&led_rgb_mutex, K_FOREVER);
	pixel.r = r;
	pixel.g = g;
	pixel.b = b;
	int rc = led_strip_update_rgb(strip, &pixel, 1);
	if (rc)
	{
		LOG_ERR("couldn't update strip: %d", rc);
	}
	is_blink_indicator_allowed = false;
	k_mutex_unlock(&led_rgb_mutex);
	return rc;
}

int led_color_blink_indicator(enum led_blink state)
{
	if (device_is_ready(strip) == false)
	{
		LOG_ERR("Led strip not ready");
		return -ENODEV;
	}
	if (is_blink_indicator_allowed == false)
	{
		return -EACCES;
	}

	k_mutex_lock(&led_rgb_mutex, K_FOREVER);
	if (state == LED_BLINK_OFF)
	{
		pixel = colors[LED_COLOR_OFF];
	}
	else
	{
		pixel = colors[LED_COLOR_BLUE];
	}
	int rc = led_strip_update_rgb(strip, &pixel, 1);
	if (rc)
	{
		LOG_ERR("couldn't update strip: %d", rc);
	}
	k_mutex_unlock(&led_rgb_mutex);
	return rc;
}

bool led_color_is_blink_allowed(void)
{
	return is_blink_indicator_allowed;
}

void led_color_allow_blink(void)
{
	is_blink_indicator_allowed = true;
}