#include "cli_handler.h"
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "../drivers/led_color.h"

#include "../log/logger.h"
LOG_MODULE_REGISTER(cli_handler, LOG_LEVEL_DEFAULT);

#define PRINT_STRING_MAX 128

static bool cli_printf(response_cb write_handler, const char *fmt, ...);

static void print_help_message(response_cb write_handler)
{
    cli_printf(write_handler, "\r\nHello there!\r\n");
    cli_printf(write_handler, "Here is a demo program to demonstrate zephyr\r\n");
    cli_printf(write_handler, "ecosystem functionality for ESP32 chip\r\n\r\n");
    cli_printf(write_handler, "Available commands:\r\n");
    cli_printf(write_handler, " help - Display this help message\r\n");
    cli_printf(write_handler, " led <color> - Set the LED color. Available colors: red, green, blue, white, off\r\n\r\n");
}

static bool cli_printf(response_cb write_handler, const char *fmt, ...)
{
    char formatted_message[PRINT_STRING_MAX] = "";
    va_list args;
    va_start(args, fmt);
    vsnprintf(formatted_message, sizeof(formatted_message), fmt, args);
    va_end(args);

    size_t length = strlen(formatted_message);
    write_handler(formatted_message, length);
    return true;
}

static bool starts_with(const char *prefix, const char *command, size_t cmd_len)
{
    size_t p_len = strlen(prefix);
    if (p_len > cmd_len) {
        return false;
    }
    return (strncmp(command, prefix, p_len) == 0);
}

static bool equals_to(const char *str, const char *command, size_t cmd_len)
{
    size_t s_len = strlen(str);
    if (s_len != cmd_len) {
        return false;
    }
    return (strncmp(command, str, s_len) == 0);
}

static void color_handle_result(response_cb write_handler, char *color_text, uint32_t length_color, enum led_color color)
{
    int rc = led_color_set_color(color);
    if (rc < 0)
    {
        LOG_ERR("couldn't set color %.*s: %d", length_color, color_text, rc);
    }
    else
    {
        cli_printf(write_handler, "LED color set to %.*s\r\n", length_color, color_text);
    }
}

int cli_handler_process_command(char *command, uint32_t length, response_cb write_handler)
{
    if (equals_to("help", command, length))
    {
        print_help_message(write_handler);
        return 1;
    }

    if (starts_with("led ", command, length))
    {
        uint32_t skip_len = strlen("led ");
        char *color_text = command + skip_len;
        uint32_t length_color = length - skip_len;
        if (equals_to("red", color_text, length_color))
        {
            color_handle_result(write_handler, color_text, length_color, LED_COLOR_RED);
        }
        else if (equals_to("green", color_text, length_color))
        {
            color_handle_result(write_handler, color_text, length_color, LED_COLOR_GREEN);
        }
        else if (equals_to("blue", color_text, length_color))
        {
            color_handle_result(write_handler, color_text, length_color, LED_COLOR_BLUE);
        }
        else if (equals_to("off", color_text, length_color))
        {
            color_handle_result(write_handler, color_text, length_color, LED_COLOR_OFF);
        }
        else if (equals_to("white", color_text, length_color))
        {
            int rc = led_color_set_rgb(30, 30, 30); // example RGB values
            if (rc < 0)
            {
                LOG_ERR("couldn't set color white: %d", rc);
            }
            else
            {
                cli_printf(write_handler, "LED turned WHITE\r\n");
            }
        }
        else
        {
            LOG_ERR("Unknown LED color command");
        }

        return 1;
    }

    LOG_ERR("Command is not recognised!");
    return 0;
}