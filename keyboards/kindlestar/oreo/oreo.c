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

// 其实是走线方向！
led_config_t g_led_config = {{
    {0,     1,        2,       3},
    {7,     6,       5,        4},
    {8,     9,      10,       11},
    {14,    13,      12,     NO_LED},
    {15,    16,      17,       NO_LED},
    {20,    NO_LED,   19,      18}
},
// x = 224 / (NUMBER_OF_COLS - 1) * COL_POSITION
// y = 64 / (NUMBER_OF_ROWS - 1) * ROW_POSITION
{
{32, 10}, {96, 10}, { 160, 10}, { 224, 10},
{224, 21}, {160, 21}, {96, 21}, {32, 21},
{32, 32}, {96, 32}, {160, 32},  {224, 37},
{160, 43}, {96, 43}, {32, 43},
{32, 54}, {96, 54}, {160, 54},
{224, 58}, {96, 63}, {32, 63},
},
{
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1,
    1, 1, 1, 1,
    1, 1
}};

/*
原始数据
9*0.773，48*0.773，66*0.773，85*0.773，105*0.773，132*0.773，152*0.773，171*0.773，191*0.773，219*0.773，238*0.773，257*0.773，276*0.773，300*0.773，319*0.773，338
9*0.773，28*0.773，47*0.773，65*0.773，85*0.773，105*0.773，123*0.773，142*0.773，162*0.773，180*0.773，200*0.773，219*0.773，238*0.773，266*0.773，300*0.773，319*0.773，338
14*0.773，38*0.773，58*0.773，76*0.773，95*0.773，114*0.773，133*0.773，153*0.773，172*0.773，190*0.773，210*0.773，229*0.773，247*0.773，272*0.773，300*0.773，319*0.773，338
7*0.773，42*0.773，62*0.773，81*0.773，100*0.773，119*0.773，138*0.773，157*0.773，176*0.773，195*0.773，214*0.773，233*0.773，264
21*0.773，52*0.773，71*0.773，90*0.773，110*0.773，129*0.773，147*0.773，167*0.773，85*0.773，204*0.773，224*0.773，259*0.773，319
12*0.773，35*0.773，59*0.773，130*0.773，202*0.773，m26*0.773，250*0.773，273*0.773，300*0.773，319*0.773，338
*/


const matrix_row_t matrix_mask[] = {
    0b01111,
    0b01111,
    0b01111,
    0b01111,
    0b01111,
    0b01111
};
