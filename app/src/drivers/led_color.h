
#ifndef H_LED_COLOR_
#define H_LED_COLOR_

#include <stdint.h>
#include <stdbool.h>

enum led_color {
    LED_COLOR_RED,
    LED_COLOR_GREEN,
    LED_COLOR_BLUE,
    LED_COLOR_OFF
};

enum led_blink {
    LED_BLINK_ON,
    LED_BLINK_OFF
};

int led_color_set_color(enum led_color color);
int led_color_set_rgb(uint8_t r, uint8_t g, uint8_t b);
int led_color_blink_indicator(enum led_blink state);
bool led_color_is_blink_allowed(void);
void led_color_allow_blink(void);

#endif /* H_LED_COLOR_ */
