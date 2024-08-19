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
#include "oreo.h"
#include "rgb_matrix.h"

int RGB_current_mode;
#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
extern rgblight_status_t rgblight_status;
void nrfmicro_power_enable(bool enable);
#endif
uint16_t last_press_time = 0;
#ifdef OS_CYCLE
uint8_t current_os=0;
#endif

bool in_gaming_mode = false;

enum custom_keycodes {
    // BLE keys
    SEL_USB = USER00,	  /* Select USB HID Sending		  */
    ADV_ID0,              /* Start advertising to PeerID 0        */
    ADV_ID1,              /* Start advertising to PeerID 1        */
    ADV_ID2,              /* Start advertising to PeerID 2        */
    ADV_ID3,              /* Start advertising to PeerID 3        */
    ADV_ID4,              /* Start advertising to PeerID 4        */
    ADV_ID5,              /* Start advertising to PeerID 5        */
    SEL_RDOE,     /* select radio hid sending */
    BLE_RBD,             /* rebond current         */
    DELBNDS,              /* Delete all bonding                   */
    TOG_GAM,              // gaming mode
    ENT_SLP,              /* Deep sleep mode                      */
    REC_DFU,  // enter radio dfu mode

    BATT_LV,              /* Display battery level in milli volts */
    DEL_ID0,              /* Delete bonding of PeerID 0           */
    DEL_ID1,              /* Delete bonding of PeerID 1           */
    DEL_ID2,              /* Delete bonding of PeerID 2           */
    DEL_ID3,              /* Delete bonding of PeerID 3           */
    DEL_ID4,              /* Delete bonding of PeerID 4           */
    DEL_ID5,              /* Delete bonding of PeerID 5           */
    AD_WO_L,		  /* Start advertising without whitelist  */
    TESTMOD,
    RE_BOOT,

    CRGBRST,
    OSSWTCH,
    CUR_MOD,  /* current mode  */

    NORM
};

enum { TO_SETTINGS=0 };

extern keymap_config_t keymap_config;
// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN,
    _extra0,
    _extra1,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC, MO(_FN),  KC_DEL,  KC_BSPC,
        KC_NLCK,KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7,  KC_P8,   KC_P9,   KC_PPLS,
        KC_P4,  KC_P5,   KC_P6,
        KC_P1,  KC_P2,   KC_P3,
        KC_P0,  LT(1,KC_PDOT),  KC_PENT),
    [1] = LAYOUT(
        ENT_SLP, _______, RGB_VAD, RGB_VAI,
        RGB_HUD, RGB_HUI, RGB_RMOD, RGB_MOD,
        SEL_RDOE, _______, BLE_RBD, RGB_TOG,
        ADV_ID3, ADV_ID4, ADV_ID5,
        ADV_ID0, ADV_ID1, ADV_ID2,
        SEL_USB,          DELBNDS, _______),
    [2] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______,
        _______,          _______, _______),
    [3] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______,
        _______,          _______, _______),
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

static bool sleeping = false;
static bool process_record_user_special(uint16_t keycode, keyrecord_t *record) {
  bool pressed = record->event.pressed;
  switch (keycode) {
  case NORM:
    if (pressed) {
      dprintf("enter NORM\n");
      set_single_persistent_default_layer(_BASE);
    }
    break;
  case DELBNDS:
    if (pressed) {
      dprintf("delete bonds\n");
      perform_cmd(KBD_CMD_BLE_DELETE_ALL_BONDS);
    }
    break;
  case AD_WO_L:
    // fixme
    break;
  case SEL_USB:
    if (pressed) {
        uprintf("sel usb\n");
        SEND_MODE old_mode = current_send_mode();
        if (old_mode == SEND_MODE_USB)
        {
            break;
        }
        // 从其他模式切换到USB，直接让nrf休眠，此时有小概率nrf没用启动，用其他手段兜底
        update_send_mode(SEND_MODE_USB);
        start_selection_rgb_animation();
        ask_nrf_sleep();
    }
    break;
  case ADV_ID0:
  case ADV_ID1:
  case ADV_ID2:
  case ADV_ID3:
  case ADV_ID4:
  case ADV_ID5:
    if (pressed) {
        SEND_MODE old_mode = current_send_mode();
        SEND_MODE new_mode = (keycode - ADV_ID0) + SEND_MODE_BLE_CHANNEL_1;
        if (old_mode == new_mode)
        {
            break;
        }
        update_send_mode(new_mode);
        start_selection_rgb_animation();
        // 从USB切蓝牙或2.4g，直接唤醒nrf即可
        if (old_mode == SEND_MODE_USB) {
            // 有可能是一开始就没睡觉，这样没法重启了
            reboot_nrf();
            wakeup_nrf();
        } else {
            reboot_nrf();
        }
        uprintf("sel from %d to ble %d\n",old_mode, new_mode);
    }
    break;
  case SEL_RDOE:
    if (pressed) {
        SEND_MODE old_mode = current_send_mode();
        SEND_MODE new_mode = SEND_MODE_RADIO;
        if (old_mode == new_mode)
        {
            break;
        }

        uprintf("sel from %d to radio %d\n",old_mode, new_mode);
        update_send_mode(new_mode);
        start_selection_rgb_animation();
        if (old_mode == SEND_MODE_USB) {
            // 有可能是一开始就没睡觉，这样没法重启了
            reboot_nrf();
            // 指令没法响应，就可以走wakeup了
            wakeup_nrf();
        } else {
            reboot_nrf();
        }
    }
    break;
  case DEL_ID0:
  case DEL_ID1:
  case DEL_ID2:
  case DEL_ID3:
  case DEL_ID4:
  case DEL_ID5:
    if (pressed) {
      dprintf("del bonds\n");
      // delete_bond_id(keycode-DEL_ID0);
    }
  case BLE_RBD:
    if (pressed) {
        perform_cmd(KBD_CMD_BLE_REBOND_CURRENT);
    }
    break;
  case CUR_MOD:
    if (pressed) {
    }
    break;
  case ENT_SLP:
    if (pressed) {
        enter_standby_mode();
    }
    break;
case BATT_LV:
    if (pressed) {
      break;
    }
case REC_DFU:
    if (pressed) {
        perform_cmd(KBD_CMD_RECEIVER_DFU);
        break;
    }

case TESTMOD:
    if (pressed)
    {
        if (sleeping)
        {
            uprintf("should wake up\n");
            // wake up sleeped nrf
            writePinHigh(B10);
            init_send_mode();
        } else {
            uprintf("should go to sleep\n");
            enter_standby_mode();
        }
        sleeping = !sleeping;
    }

    break;
   case RE_BOOT:
   if (pressed) {
    perform_cmd(KBD_CMD_REBOOT);
   }
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
