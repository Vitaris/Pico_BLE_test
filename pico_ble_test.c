#include "pico/stdlib.h"
#include <string.h>
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "lcd.h"

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
    lcd_1 = lcd_create(8, 9, 10, 11, 12, 13, 14, 16, 2);
    while (1) {
        if (lcd_refresh == true) {
            string2LCD(lcd_1, 0, 0, "LCD_1");
        }
    }
}

bool LCD_refresh_timer_callback(struct repeating_timer *t) {
    lcd_refresh = true;
    return true;
}

int main() {
    // 100ms LCD refresh timer
    add_repeating_timer_ms(-100, LCD_refresh_timer_callback, NULL, &LCD_refresh_timer);
    
    // Launch core1
    multicore_launch_core1(core1_entry);

    while (1) {
        tight_loop_contents();
    }
}
