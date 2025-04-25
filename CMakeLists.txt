cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico_ble_test)

pico_sdk_init()

add_executable(pico_ble_test
    pico_ble_test.c
    lcd.c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(pico_ble_test 1)
pico_enable_stdio_uart(pico_ble_test 0)

pico_add_extra_outputs(pico_ble_test)

target_link_libraries(pico_ble_test pico_stdlib
        pico_multicore
        )
        