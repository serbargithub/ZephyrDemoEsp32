
#ifndef H_LED_COLOR_
#define H_LED_COLOR_


#include <stdint.h>

enum led_color {
	LED_COLOR_RED,
	LED_COLOR_GREEN,
	LED_COLOR_BLUE,
	LED_COLOR_OFF
};

int led_color_set_color(enum led_color color);
int led_color_set_rgb(uint8_t r, uint8_t g, uint8_t b);


#endif /* H_LED_COLOR_ */
