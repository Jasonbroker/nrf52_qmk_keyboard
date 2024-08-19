#ifdef RGB_MATRIX_ENABLE

#include "kindlestar_custom_animation.h"

#include <stdint.h>
#include "progmem.h"
#include "rgb_matrix.h"

static uint8_t divider = 3;
static uint8_t dulplicator = 0;


const uint8_t custom_rgblight_effect_breathe_table[] PROGMEM = {
    0x22,
    0x33,
    0x44,
    0x55,
    0x66,
    0x77,
    0x88,
    0x99,
    0xaa,
    0xbb,
    0xcc,
    0xdd,
    0xee,
    0xff,
    0xff,
    0xff,
    0xff,
    0xee,
    0xdd,
    0xcc,
    0xbb,
    0xaa,
    0x99,
    0x88,
    0x77,
    0x66,
    0x55,
    0x44,
    0x33,
    0x22,
};

typedef enum
{
    keystar_custom_anim_single_breath,
    keystar_custom_anim_type_select_radio_performace,
    keystar_custom_anim_type_low_power,
    keystar_custom_anim_type_battery_indication,
} keystar_custom_anim_type;

typedef struct
{
    bool playing;
    keystar_custom_anim_type type;
    uint8_t ticks_per_round;    //
    uint16_t all_ticks;          // % round count
    uint16_t tick_idx;
    uint8_t rgb_idx;
    animation_finish_handler handler;
} ks_anmation_config;

ks_anmation_config custom_config;

void __blink_blue_animation_type(keystar_custom_anim_type type, uint8_t idx, void *handler)
{
  custom_config.playing = true;
  custom_config.ticks_per_round    = sizeof(custom_rgblight_effect_breathe_table);
  custom_config.all_ticks         = custom_config.ticks_per_round * 3;
  custom_config.tick_idx          = 0;
  custom_config.type              = type;
  custom_config.rgb_idx = idx;
  custom_config.handler = handler;
}

void start_breath_at_idx(uint8_t idx, animation_finish_handler handler)
{
  __blink_blue_animation_type(keystar_custom_anim_single_breath, idx, handler);
}

void keystar_low_power_animation(void)
{
//   // rgblight_timer_disable();
//   custom_config.playing = true;
//   custom_config.ticks_per_round    = sizeof(custom_rgblight_effect_breathe_table);
//   custom_config.all_ticks         = custom_config.ticks_per_round * 10;
//   custom_config.tick_idx          = 0;
//   custom_config.type              = keystar_custom_anim_type_low_power;
}


void __single_blink_animation_func(uint8_t idx, int8_t led_min, uint8_t led_max);
void __whole_board_breath_func(void);
void __battery_indication_func(void);

bool custom_animation_task(uint8_t led_min, uint8_t led_max)
{
  if (!custom_config.playing) return false;
  if (custom_config.tick_idx >= custom_config.all_ticks)
  {
      custom_config.playing = false;
      custom_config.tick_idx = 0;
      dulplicator = 0;
      custom_config.handler();
      return false;
  }

  if (dulplicator == 0) {
      dulplicator++;
  } else {
    dulplicator++;
    dulplicator %=divider;
    return true;
  }

  if (custom_config.type == keystar_custom_anim_single_breath)
  {
    __single_blink_animation_func(custom_config.rgb_idx, led_min, led_max);
  } else if (custom_config.type == keystar_custom_anim_type_low_power) {
    __whole_board_breath_func();
  } else if (custom_config.type == keystar_custom_anim_type_battery_indication) {
    __battery_indication_func();
  }
  custom_config.tick_idx++;

  return true;
}

// #include "color.h"

void __single_blink_animation_func(uint8_t idx, int8_t led_min, uint8_t led_max)
{
    rgb_matrix_set_color(idx, 0, 0, custom_rgblight_effect_breathe_table[custom_config.tick_idx % custom_config.ticks_per_round]);
    // // rgb_matrix_set_color(idx, 0, 0, custom_rgblight_effect_breathe_table[custom_config.tick_idx % custom_config.ticks_per_round]);
    // for (uint8_t i = led_min; i <= led_max; i++) {
    //     if (i == idx) {

    //     } else {
    //         rgb_matrix_set_color(i, RGB_OFF);
    //     }
    // }
}

void __whole_board_breath_func(void)
{
}

void __battery_indication_func(void)
{
  // do nothing
}

// test only
void keystar_battery_indication(uint8_t percentage)
{
}

#endif
