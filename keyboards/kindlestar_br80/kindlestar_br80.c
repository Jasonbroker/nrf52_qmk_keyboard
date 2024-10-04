
#include "gpio.h"
#include "quantum.h"

void keyboard_post_init_user(void) {
    gpio_set_pin_input(B8);
}

#define HIGH 1
#define LOW 0
static bool last_state = LOW;
static uint64_t last_state_duration = 0;

void housekeeping_task_user(void) {
    if(gpio_read_pin(B8) == HIGH) {
        if (last_state == LOW)
        {
            last_state = HIGH;
            last_state_duration = 1;
        } else {
            last_state_duration++;
        }
    } else {
        last_state = LOW;
        last_state_duration = 0;
    }

    if (last_state == HIGH && last_state_duration == 100) {
        // 执行重启
        soft_reset_keyboard();
    } else if (last_state == HIGH && last_state_duration == 10000) {
        // 执行reset，刷机
        reset_keyboard();
    }
}
