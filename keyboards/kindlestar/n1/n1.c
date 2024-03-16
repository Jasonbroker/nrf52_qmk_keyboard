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
        {0,     1,        2,        3,      4,       5,       6,       7,        8,        9,      10,        11,     12,    13,   14},
        {32,    31,      30,       29,     28,       27,       26,     25,       24,      23,      22,        21,     20,    19,   15},
        {33,    34,      35,       36,     37,       38,      39,      40,       41,      42,      43,        44,     45,    46,   18},
        {62,    61,      60,       59,     58,       57,      56,      55,       54,      53,      52,        51,     50,    17,   16},
        {63,    64,      65,       66,     67,       68,      69,      70,      71,      72,      73,         74,     75,    47,   48},
        {86,    85,      84,       NO_LED, NO_LED,   83,      NO_LED,  82,       81,      80,      79,       78,     77,     76,   49}
},
// x = 224 / (NUMBER_OF_COLS - 1) * COL_POSITION
// y = 64 / (NUMBER_OF_ROWS - 1) * ROW_POSITION
{
{  6, 10}, {35, 10}, { 48, 10}, { 62, 10}, { 76, 10}, { 96, 10}, {111, 10},{125, 10},{140, 10},{160, 10},{174, 10},{188, 10},{202, 10},{219, 10},{233, 10},{247, 10},
{247, 21}, {233, 21}, {219, 21},{194, 21},{174, 21},{160, 21},{146, 21},{131, 21},{118, 21},{104, 21},{90, 21},{76, 21},{62, 21},{47, 21},{34, 21},{20, 21},{6, 21},
{10, 32},{27, 32},{42, 32},{55, 32},{69, 32},{83, 32},{97, 32},{112, 32},{126, 32},{139, 32},{153, 32},{167, 32},{181, 32},{199, 32},{219, 32},{233, 32},{247, 32},
{193, 43},{170, 43},{156, 43},{142, 43},{129, 43},{115, 43},{101, 43},{87, 43},{73, 43},{59, 43},{45, 43},{30, 43},{5, 43},
{15, 54},{38, 54},{52, 54},{65, 54},{80, 54},{94, 54},{107, 54},{122, 54},{135, 54},{149, 54},{164, 54},{189, 54},{233, 54},
{247, 63},{233, 63},{219, 63},{200, 63},{183, 63},{165, 63},{148, 63},{95, 63},{43, 63},{25, 63},{8, 63}
},
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
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
    0b0111111111111101,
    0b0111111111111111,
    0b0111111111111111,
    0b0111111111111111,
    0b0111111111111111,
    0b0111111111111111,
};


// code for adc initialization

#include "ch.h"
#include "hal.h"
void keyboard_pre_init_user(void) {
    // add adc configration
    // https://chibiforge.org/doc/21.6/hal/group___a_d_c.html
    // https://www.playembedded.org/blog/stm32-adc-chibios/
    /*
   * Setting up analog inputs used by the demo.
   */
  palSetGroupMode(GPIOC, PAL_PORT_BIT(0) | PAL_PORT_BIT(1),
                  0, PAL_MODE_INPUT_ANALOG);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Activates the ADC1 driver and the temperature sensor.
   */
  adcStart(&ADCD1, NULL);

  /*
   * Linear conversion.
   */
  adcConvert(&ADCD1, &adcgrpcfg1, samples1, ADC_GRP1_BUF_DEPTH);
  chThdSleepMilliseconds(1000);

  /*
   * Starts an ADC continuous conversion.
   */
  adcStartConversion(&ADCD1, &adcgrpcfg2, samples2, ADC_GRP2_BUF_DEPTH);


}
