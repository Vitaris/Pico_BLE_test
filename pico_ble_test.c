#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "lcd.h"

#define BUTTON_GPIO_0 3
#define BUTTON_GPIO_1 4


// Timers
uint64_t old_cycle_time = 0;
struct repeating_timer LCD_refresh_timer;

// LCD
lcd_t lcd_1;
lcd_t lcd_2;
lcd_t lcd_3;

bool lcd_refresh;

void core1_entry() {

    // LCD
    lcd_1 = lcd_create(9, 10, 11, 12, 13, 14, 15, 16, 2);
    while (1) {
        if (lcd_refresh == true) {
            bool switch_0 = gpio_get(BUTTON_GPIO_0) == 0; // active low
            bool switch_1 = gpio_get(BUTTON_GPIO_1) == 0; // active low
            int combined = (switch_1 << 1) | switch_0;

            char lcd_line[16];
            snprintf(lcd_line, sizeof(lcd_line), "LCD %d", combined);
            string2LCD(lcd_1, 0, 0, lcd_line);

            lcd_refresh = false;
        }
    }
}

bool LCD_refresh_timer_callback(struct repeating_timer *t) {
    lcd_refresh = true;
    return true;
}

int main() {
    gpio_init(BUTTON_GPIO_0);
    gpio_set_dir(BUTTON_GPIO_0, GPIO_IN);
    gpio_pull_up(BUTTON_GPIO_0);

    gpio_init(BUTTON_GPIO_1);
    gpio_set_dir(BUTTON_GPIO_1, GPIO_IN);
    gpio_pull_up(BUTTON_GPIO_1);

    // 100ms LCD refresh timer
    add_repeating_timer_ms(-100, LCD_refresh_timer_callback, NULL, &LCD_refresh_timer);
    
    // Launch core1
    multicore_launch_core1(core1_entry);

    while (1) {
        tight_loop_contents();
    }
}
