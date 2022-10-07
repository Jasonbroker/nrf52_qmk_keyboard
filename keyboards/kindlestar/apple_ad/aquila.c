/* Copyright 2021 @ KindleStar (https://www.KindleStar.com)
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

#include "quantum.h"
#ifdef RGB_MATRIX_ENABLE
// 其实是走线方向！
led_config_t g_led_config = {{
        {13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0},
        {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27},
        {40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, NO_LED},
        {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, NO_LED},
        {63, 62, 61,NO_LED,NO_LED,60, NO_LED,NO_LED, 59, 58, 57, 56, 55, 54}
},
// x = 224 / (NUMBER_OF_COLS - 1) * COL_POSITION
// y = 64 / (NUMBER_OF_ROWS - 1) * ROW_POSITION
// 这里就是正常的未知排序，不用特别适配
{
{6, 13}, {19, 13},   {32, 13},  { 45, 13}, { 58, 13}, { 71, 13}, { 84, 13}, {97, 13},{110, 13},{123, 13},{136, 13},{149, 13},{162, 13},{182, 13},
{185, 26}, {169, 26}, {156, 26}, {143, 26}, {130, 26}, {117, 26}, {104, 26},{91, 26},{78, 26},{64, 26},{51, 26},{38, 26},{25, 26}, {9, 26},
{5, 39},   {29, 39}, {42, 39},  {55, 39},  {68, 39},  {81, 39},  {94, 39}, {107, 39},{120, 39},{133, 39},{146, 39},{159, 39},{180, 39},
{205, 52}, {177, 52},{153, 52}, {140, 52}, {127, 52}, {113, 52}, {100, 52},{87, 52},  {74, 52}, {61, 52}, {48, 52}, {35, 52}, {14, 52},
{8, 64}, {24, 64},   {40, 64}, {89, 64}, {138, 64},  {154, 64}, {171, 64}, {192, 64}, {205, 64}, {218, 64}
},
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
}};

// {6, 13}, {19, 13},   {32, 13},  { 45, 13}, { 58, 13}, { 71, 13}, { 84, 13}, {97, 13},{110, 13},{123, 13},{136, 13},{149, 13},{162, 13},{182, 13},
// {210, 26}, {192, 26}, {177, 26}, {162, 26}, {147, 26}, {132, 26}, {118, 26},{103, 26},{88, 26},{73, 26},{59, 26},{44, 26},{29, 26}, {10, 26},
// {5, 39},   {32, 39}, {47, 39},  {62, 39},  {77, 39},  {92, 39},  {106, 39}, {121, 39},{136, 39},{151, 39},{166, 39},{180, 39},{205, 39},
// {232, 52}, {201, 52},{173, 52}, {158, 52}, {143, 52}, {129, 52}, {114, 52},{16, 52},  {99, 52}, {84, 52}, {69, 52}, {55, 52}, {40, 52},
// {8, 64}, {27, 64},   {45, 64}, {101, 64}, {156, 64},  {175, 64}, {193, 64}, {217, 64}, {232, 64}, {247, 64}

const matrix_row_t matrix_mask[] = {
    0b0111111111111111,
    0b0111111111111111,
    0b0111111111111111,
    0b0111111111111111,
    0b0111111111111111,
};

#endif

#ifdef ENCODER_ENABLE
#include "action_layer.h"

bool encoder_update_kb(uint8_t index, bool clockwise) {

    uint8_t current_layer = biton32(layer_state);
    // uprintf("turn %d, cl %d\n", clockwise, current_layer);
    if (clockwise) {
        // tap_code(dynamic_keymap_get_keycode(current_layer, 4, 7), 10);
        tap_code_delay(dynamic_keymap_get_keycode(current_layer, 4, 7), 2);
    } else {
        tap_code_delay(dynamic_keymap_get_keycode(current_layer, 4, 6), 2);
    }
    return false;
}

#endif
