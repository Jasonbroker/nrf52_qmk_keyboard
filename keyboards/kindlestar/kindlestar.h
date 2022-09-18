/* Copyright 2021 @ kindlestar (https://www.KindleStar.com)
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

#pragma once

#include "quantum.h"

typedef enum {
  SEND_MODE_USB = 0,
  SEND_MODE_BLE_CHANNEL_1 = 1,
  SEND_MODE_BLE_CHANNEL_2 = 2,
  SEND_MODE_BLE_CHANNEL_3 = 3,
  SEND_MODE_BLE_CHANNEL_4 = 4,
  SEND_MODE_BLE_CHANNEL_5 = 5,
  SEND_MODE_BLE_CHANNEL_6 = 6,
  SEND_MODE_RADIO = 7,
} SEND_MODE;

typedef enum {
    KBD_SEND_CMD_START = 1,
    KBD_SEND_CMD_REPORT_ID_KEYBOARD = KBD_SEND_CMD_START << 3,
    KBD_SEND_CMD_REPORT_ID_MOUSE = 2 << 3,
    KBD_SEND_CMD_REPORT_ID_SYSTEM = 3 << 3,
    KBD_SEND_CMD_REPORT_ID_CONSUMER = 4 << 3,
    KBD_SEND_CMD_REPORT_ID_NKRO = 5 << 3,
    KBD_SEND_CMD_REPORT_ID_JOYSTICK = 6 << 3,
    KBD_SEND_CMD_REPORT_ID_DIGITIZER = 7 << 3,


    KBD_CMD_START = 8,
    KBD_CMD_BLE_REBOND_CURRENT = KBD_CMD_START << 3,
    KBD_CMD_BLE_DELETE_ALL_BONDS = 9 << 3,
    KBD_CMD_SLEEP = 10 << 3,
    KBD_CMD_REBOOT = 11 << 3,
    KBD_CMD_RECEIVER_DFU = 12 << 3,
    KBD_CMD_TEST = 13 << 3,

    KBD_SELECT_CMD_USB = 20 << 3,
    KBD_SELECT_CMD_BLE1 = KBD_SELECT_CMD_USB + SEND_MODE_BLE_CHANNEL_1,
    KBD_SELECT_CMD_BLE2 = KBD_SELECT_CMD_USB + SEND_MODE_BLE_CHANNEL_2,
    KBD_SELECT_CMD_BLE3 = KBD_SELECT_CMD_USB + SEND_MODE_BLE_CHANNEL_3,
    KBD_SELECT_CMD_BLE4 = KBD_SELECT_CMD_USB + SEND_MODE_BLE_CHANNEL_4,
    KBD_SELECT_CMD_BLE5 = KBD_SELECT_CMD_USB + SEND_MODE_BLE_CHANNEL_5,
    KBD_SELECT_CMD_BLE6 = KBD_SELECT_CMD_USB + SEND_MODE_BLE_CHANNEL_6,
    KBD_SELECT_CMD_RADIO = KBD_SELECT_CMD_USB + SEND_MODE_RADIO,

} KBD_CMD;

enum {
    KBD_CMD_REQUEST_MODE = 20 << 3,   // 希望同步一下模式
    KBD_CMD_REQUEST_CAPS_OFF = 1,     // caps 关闭
    KBD_CMD_REQUEST_CAPS_ON = 2,      // caps 打开

    KBD_CMD_RESPONSE_SUCCESS = 0x66, // success
    KBD_CMD_RESPONSE_FAILED = 0xba  // failed
};

void perform_cmd(KBD_CMD cmd);

void init_send_mode(void);

SEND_MODE current_send_mode(void);

void update_send_mode(SEND_MODE mode);

void enter_sleep_mode(void);

void enter_stop_mode(void);

void enter_standby_mode(void);

void ask_nrf_sleep(void);

void wakeup_nrf(void);

void reboot_nrf(void);

void start_selection_rgb_animation(void);

// static uint8_t power_enbled_cnt = 0;
#define ENABLE_POWER() writePinHigh(B11)

#define DISABLE_POWER() writePinLow(B11)
