
ESP32-C3 WS2812 LED Strip Demo (Zephyr)
=====================================

Short description
-----------------
Simple Zephyr demo that drives a WS2812 LED strip on an ESP32-C3 DevKitM.  
Provides a UART console (TTY) with a minimal CLI to set LED colors and has a small input history (arrow-up/arrow-down).

Features
--------
- WS2812 LED strip support via devicetree node and Zephyr LED strip API
- CLI over UART console for setting LED colors and turning off
- UART console with line editing and history
- Safe printf helper for CLI responses
- Fallback behavior available if the LED strip device is not present
- Concurrency protection for LED operations (Zephyr mutexes)


Requirements
------------
- Zephyr SDK and west installed and configured
- Board: esp32c3_devkitm (target used in examples)

Continuous Integration
----------------------
This project uses GitHub Actions for automated builds and code quality checks:

- **Build verification**: Compiles the project for esp32c3_devkitm
- **Code formatting**: Validates code style using clang-format-14
- **Pull request checks**: Runs on all PRs and pushes

See ``.github/workflows/ci.yml`` for the complete CI configuration.

Files of interest
-----------------
- boards/esp32c3_devkitm.overlay — device tree overlay for the LED strip & console pinctrl
- prj.conf — Kconfig for project features (console, logging, LED strip base)
- src/main.c — application entry
- src/drivers/led_color.c — LED helper/driver for setting colors and blinking
- src/console/* — UART console, CLI and command processing

Quick start
-----------
From project root:

.. code-block:: bash

    # clean and build
    rm -rf build
    west build -b esp32c3_devkitm -p always

    # flash (if supported)
    west flash

CLI usage
---------
Connect to serial (115200). Example commands:

- ``help`` — show help
- ``led red`` / ``led green`` / ``led blue`` / ``led white`` / ``led off``

