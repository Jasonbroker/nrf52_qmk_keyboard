/* Copyright 2021 zhengchangzhou
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H



#include "keycode_config.h"
#include "keymap.h"
#include "k1.h"
#include "config.h"

uint16_t last_press_time = 0;
#ifdef OS_CYCLE
uint8_t current_os=0;
#endif

bool in_gaming_mode = false;

enum custom_keycodes {
    // BLE keys
    TOG_GAM,              // gaming mode
    ENT_SLP,              /* Deep sleep mode                      */
    CRGBRST,
    OSSWTCH,
    NORM
};

enum { TO_SETTINGS=0 };

extern keymap_config_t keymap_config;
// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _Mac,
    _FN,
    _extra,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,       KC_9,           KC_0,    KC_MINS, KC_EQL,    KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,       KC_O,           KC_P,    KC_LBRC, KC_RBRC,   KC_BSLS, KC_HOME,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,       KC_L,           KC_SCLN, KC_QUOT, KC_ENT,    KC_PGUP,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM,    KC_DOT,         KC_SLSH, KC_RSFT, KC_UP,     KC_PGDN,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                    KC_RALT,    MO(_FN),        KC_RCTL, KC_LEFT, KC_DOWN,   KC_RGHT
    ),
    [_Mac] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,        _______, _______, _______,  _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,        _______, _______, _______,  _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,        _______, _______, _______,  _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,        _______, _______, _______,  _______,
        _______, KC_LALT, KC_LGUI,                   _______,                   _______,    _______,        _______, _______, _______,  _______
    ),
    [_FN] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______,_______,    _______,        _______, _______, _______,  _______, _______,
        _______, _______, _______, _______, QK_BOOT,   _______, _______, _______, _______,    CRGBRST,        _______, RGB_SAI, RGB_SAD,  _______, ENT_SLP,
        _______, _______, _______, _______, _______, TOG_GAM, TG(0),   TG(1),   _______, QK_BACKLIGHT_TOGGLE, RGB_HUI, RGB_HUD, _______,  _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,        _______, _______, QK_BACKLIGHT_UP,  _______,
QK_BACKLIGHT_ON, QK_BACKLIGHT_OFF, _______,          _______,                   _______,    _______,        _______, QK_BACKLIGHT_TOGGLE_BREATHING,QK_BACKLIGHT_DOWN,  _______
    ),
    [_extra] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,        _______, _______, _______,  _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,        _______, _______, _______,  _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,        _______, _______, _______,  _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______,        _______, _______, _______,  _______,
        _______, _______, _______,                   _______,                   _______,    _______,        _______, _______, _______,  _______
    )
};

void reboot_system(void);
#ifdef RGBLIGHT_ENABLE
void log_rgb_state(void);
void lightup_current_mode(void);
#endif

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
//   debug_matrix=true;
//   debug_keyboard=true;
//   debug_mouse=true;
}

void enter_standby_mode(void);
static bool process_record_user_special(uint16_t keycode, keyrecord_t *record) {
  bool pressed = record->event.pressed;
  switch (keycode) {
  case NORM:
    if (pressed) {
      dprintf("enter NORM\n");
      set_single_persistent_default_layer(_BASE);
    }
    break;
  case ENT_SLP:
    if (pressed) {
        enter_standby_mode();
    }
    break;
  case KC_LGUI:
  case KC_RGUI:
  {
      // dont do other staff plz
      return !in_gaming_mode;
  }
  case TOG_GAM:
  {
    if (pressed) {
      in_gaming_mode = !in_gaming_mode;
    }
  }
    break;
  default:
    // other unspecial keys
    return true;
  }
  return false;
}

void eeconfig_debug_rgb_matrix(void);
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // if (kbd_state == keyboard_state_waiting_for_reboot) return true;

  switch(keycode)
  {
    case CRGBRST:
      #ifdef RGB_MATRIX_ENABLE
        if (record->event.pressed) {
            rgblight_enable();
            eeconfig_update_rgb_matrix_default();
        }
      #endif
      break;
    case RGB_TOG:
    if (!record->event.pressed) {
#ifdef RGB_MATRIX_ENABLE
        // 这里看起来很奇怪的，因为这是之前的状态，我们不去设置值，是后面他自己设置的
        if(rgblight_is_enabled()) {
            uprintf("rgb matrix will disabled\n");
            writePinLow(B11);
        } else {
            uprintf("rgb matrix will enabled\n");
            setPinOutput(B11);
            writePinHigh(B11);
        }
        eeconfig_debug_rgb_matrix();
        // 只有返回true 才会继续让rgb的切换逻辑走，这样就能更新了
        return true;
#endif
    }
    return false;
    #ifdef OS_CYCLE
    case PRINTER:
        if (record->event.pressed) {
          current_os=eeconfig_read_os();
          if(current_os==0){
            register_code(KC_0);
            unregister_code(KC_0);
          }else{
            register_code(KC_1);
            unregister_code(KC_1);
          }
        }
      break;
    case OSSWTCH:

        if (record->event.pressed) {
          current_os=eeconfig_read_os();
          if(current_os==0){
            current_os=1;
            eeconfig_update_os(current_os);
          }else{
            current_os=0;
            eeconfig_update_os(current_os);
          }
        }
      break;
    #endif
  }

  switch (keycode) {
  default:
    // unset_layer(record);
    return process_record_user_special(keycode, record);
  }
  return false;
}
