/* Copyright 2021 @ KindleStar(https://www.KindleStar.com)
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


/* USB Device descriptor parameter */
#define VENDOR_ID       0x6468
#define MANUFACTURER    IDOBAO
#define PRODUCT         IDOBAO
/* USB Device descriptor parameter */

#define PRODUCT_ID      0x0100
#define DEVICE_VER      0x0101

/* key matrix pins */
#define MATRIX_ROW_PINS {A4, A3, A2, A1, A0}
#define MATRIX_COL_PINS {A15, B3, B4, B5, B6, B7, B12, B13, B14, B15, A8, A6, A7, B0, B1}

#define LED_CAPS_LOCK_PIN B8

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 15

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#define LED_PIN_ON_STATE 0

/* NKRO */
// #define FORCE_NKRO

#define RGB_DI_PIN B9
/* RGB Matrix Configuration */

#ifdef RGB_MATRIX_ENABLE

#define DRIVER_LED_TOTAL 67

#define START_BREATH_IDX 14

#define RGB_MATRIX_STARTUP_HUE 170
#define RGB_MATRIX_STARTUP_VAL 255
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 160

#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_LED_FLUSH_LIMIT 26
#define RGB_DISABLE_WHEN_USB_SUSPENDED  // turn off effects when suspended
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_RAINBOW_BEACON

#define ENABLE_RGB_MATRIX_RAINBOW_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS

#endif

#define KEYSTAR_LED_RED_ON() {}
#define KEYSTAR_LED_GREEN_ON() {}
#define KEYSTAR_LED_BLUE_ON() {}

#define KEYSTAR_LED_RED_OFF() {}
#define KEYSTAR_LED_GREEN_OFF() {}
#define KEYSTAR_LED_BLUE_OFF() {}

// #undef VAL_GPIOCCRH
// #define VAL_GPIOCCRH            0x33388888      /* PC15...PC8 */

// #undef STM32_LSECLK
