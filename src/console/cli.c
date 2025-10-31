#include <zephyr/shell/shell.h>
#include "../drivers/led_color.h"
#include "../log/logger.h"
LOG_MODULE_REGISTER(cli);

static int cmd_info(const struct shell *shell, size_t argc, char **argv)
{
    if (argc != 1)
    {
        shell_print(shell, "Usage: info <no arguments>");
        return -EINVAL;
    }

    shell_print(shell, "\r\nHello there!");
    shell_print(shell, "Here is a demo program to demonstrate zephyr");
    shell_print(shell, "ecosystem functionality for ESP32 chip\r\n");
    shell_print(shell, "Available commands:");
    shell_print(shell, " info - Display this info message");
    shell_print(shell, " led <color> - Set the LED color. Available colors: red, green, blue, white, off");

    return 0;
}

static void color_handle_result(const struct shell *shell, char *color_text, enum led_color color)
{
    int rc = led_color_set_color(color);
    if (rc < 0)
    {
        LOG_ERR("couldn't set color %.*s: %d", strlen(color_text), color_text, rc);
    }
    else
    {
        shell_print(shell, "LED color set to %.*s", strlen(color_text), color_text);
    }
}

static int cmd_led(const struct shell *shell, size_t argc, char **argv)
{
    if (argc != 2)
    {
        shell_print(shell, "Usage: led <red|green|blue|white|off>");
        return -EINVAL;
    }

    if (strcmp(argv[1], "red") == 0)
    {
        color_handle_result(shell, argv[1], LED_COLOR_RED);
    }
    else if (strcmp(argv[1], "green") == 0)
    {
        color_handle_result(shell, argv[1], LED_COLOR_GREEN);
    }
    else if (strcmp(argv[1], "blue") == 0)
    {
        color_handle_result(shell, argv[1], LED_COLOR_BLUE);
    }
    else if (strcmp(argv[1], "white") == 0)
    {
        int rc = led_color_set_rgb(30, 30, 30); // example RGB values
        if (rc < 0)
        {
            LOG_ERR("couldn't set color white: %d", rc);
        }
        else
        {
            shell_print(shell, "LED turned WHITE");
        }
    }
    else if (strcmp(argv[1], "off") == 0)
    {
        color_handle_result(shell, argv[1], LED_COLOR_OFF);
    }
    else
    {
        shell_print(shell, "Unknown color: %s", argv[1]);
    }

    return 0;
}


// --- Commands registration ---
SHELL_CMD_REGISTER(info, NULL, "Get general info about the device", cmd_info);
SHELL_CMD_REGISTER(led, NULL, "Set LED color", cmd_led);
// -----------------------------
