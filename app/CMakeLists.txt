# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(esp32_led_strip)

# Include header files
target_include_directories(app PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_SOURCE_DIR}/src/log
      ${CMAKE_CURRENT_SOURCE_DIR}/src/console
      ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers
  )

set(APP_SOURCES 
    src/main.c
    src/drivers/led_color.c
    src/console/uart_console.c
    src/console/cli_handler.c
    src/console/uart_console_thread.c
)

target_sources(app PRIVATE ${APP_SOURCES})

