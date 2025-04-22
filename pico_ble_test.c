#include "pico/stdlib.h"
#include <string.h>
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "lcd.h"
#include "button.h"

// Timers
uint64_t old_cycle_time = 0;
struct repeating_timer LCD_refresh_timer;

// LCD
lcd_t lcd;

// Buttons
button_t F1;
button_t F2;
button_t F3;
button_t F4;

bool F1_pressed;
bool F2_pressed;
bool F3_pressed;
bool F4_pressed;
bool some_released;

bool blink_500ms;
bool lcd_refresh;

uint offset;
uint offset_encoder;

bool stepper_step;
bool stepper_dir;
bool stepper_enable;
bool toogle = false;
uint speed;

void core1_entry() {

    // LCD
    lcd = lcd_create(2, 3, 4, 5, 6, 7, 8, 16, 2);
    int enc_old = 0;
    bool led_state = false;
    bool lcd_step = false;
    bool lcd_dir = false;
    while (1)
    {
        if (lcd_refresh == true)
        {
            string2LCD(lcd, 0, 0, "Enc:");
            string2LCD(lcd, 13, 0, "rev");
            // string2LCD(lcd, 13, 1, "r/s");
            // int2LCD(lcd, 0, 1, 6, a++); 
            // float2LCD(lcd, 4, 1, 8, (float)(enc - enc_old) * 10 / 8000.0);
          
        }
    }
}

bool LCD_refresh_timer_callback(struct repeating_timer *t) {
    lcd_refresh = true;
    return true;
}

int main() {

    F1 = create_button(10);
    F2 = create_button(11);
    F3 = create_button(12);
    F4 = create_button(13);

    stepper_step = false;
    stepper_dir = false;
    stepper_enable = false;
    gpio_init(14);
    gpio_set_dir(14, GPIO_OUT);


    // 100ms LCD refresh timer
    add_repeating_timer_ms(-100, LCD_refresh_timer_callback, NULL, &LCD_refresh_timer);
    
    // Launch core1
    multicore_launch_core1(core1_entry);

    // Initial wait 
    // busy_wait_ms(500);

    while (1)
    {
        tight_loop_contents();
    }
}
