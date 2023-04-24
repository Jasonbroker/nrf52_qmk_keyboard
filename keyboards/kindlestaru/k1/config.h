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


/* key matrix pins */
#define MATRIX_ROW_PINS {B6, B7, A1, A2, A0}
#define MATRIX_COL_PINS {A3, A4, A6, A7, B0, B1, B12, B13, B14, B15, A8, A15, B3, B4, B5}

#define LED_CAPS_LOCK_PIN B8

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 15

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

#define LED_PIN_ON_STATE 0

#define BACKLIGHT_PWM_DRIVER    PWMD2
#define BACKLIGHT_PWM_CHANNEL   4
#define BACKLIGHT_PAL_MODE      2
