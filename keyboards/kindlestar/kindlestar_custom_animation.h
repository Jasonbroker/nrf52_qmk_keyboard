#pragma once

#include <stdbool.h>
#include <stdint.h>

// fixme should write to register mode
// need refraction
typedef void (*animation_finish_handler)(void);

void start_breath_at_idx(uint8_t idx, animation_finish_handler handler);

void keystar_low_power_animation(void);

bool custom_animation_task(uint8_t led_min, uint8_t led_max);

// test only
void keystar_battery_indication(uint8_t percentage);

