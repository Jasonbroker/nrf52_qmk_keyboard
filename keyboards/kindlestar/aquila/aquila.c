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

#ifdef BUBBLE_ENABLE
#include "config.h"
// #if 1
#include "hal.h"
#include "hal_soft_serial.h"


#define SSD1_BAUD                     9600
#define SSD1_BITRATE_MULTIPLIER       4
#define SSD1_TX_LINE                  PAL_LINE(GPIOA, 6)
#define SSD1_RX_LINE                  PAL_LINE(GPIOA, 5)

#define SSD1_TIMER                    STM32_TIM3
#define SSD1_TIMER_CLOCK              STM32_TIMCLK1
#define SSD1_TIMER_HANDLER            STM32_TIM3_HANDLER
#define SSD1_TIMER_IRQ_NUMBER         STM32_TIM3_NUMBER
#define SSD1_TIMER_ENABLE()           rccEnableTIM3(true)
#define SSD1_TIMER_DISABLE()          rccDisableTIM3()
#define SSD1_TIMER_RESET()            rccResetTIM3()
#define SSD1_TIMER_FREQ               SSD1_BAUD * SSD1_BITRATE_MULTIPLIER
#define SSD1_TIMER_ARR                (uint16_t)((SSD1_TIMER_CLOCK / (SSD1_TIMER_FREQ)) - 1)
#define SSD1_TIMER_IRQ_PRIORITY       5

static const SoftSerialConfig ssd1_config = {
    SSD1_BITRATE_MULTIPLIER,
    false,
    SSD1_RX_LINE,
    SSD1_TX_LINE};

SoftSerialDriver SSD1;

/*
 * TIMER SETUP
 */

OSAL_IRQ_HANDLER(SSD1_TIMER_HANDLER)
{
    OSAL_IRQ_PROLOGUE();
    SSD1_TIMER->SR = 0; // Clear pending IRQs

    osalSysLockFromISR();
    ssdTickI(&SSD1);
    osalSysUnlockFromISR();

    OSAL_IRQ_EPILOGUE();
}

static void timerStop(void)
{
    SSD1_TIMER->CR1 = 0;  // Timer disabled
    SSD1_TIMER->DIER = 0; // All IRQs disabled
    SSD1_TIMER->SR = 0;   // Clear pending IRQs

    nvicDisableVector(SSD1_TIMER_IRQ_NUMBER);
    SSD1_TIMER_DISABLE();
}

static void timerStart(void)
{
    SSD1_TIMER_ENABLE();
    SSD1_TIMER_RESET();

    nvicEnableVector(SSD1_TIMER_IRQ_NUMBER, SSD1_TIMER_IRQ_PRIORITY);

    SSD1_TIMER->CR1 = 0;  // Initially stopped
    SSD1_TIMER->CR2 = 0;  //
    SSD1_TIMER->PSC = 0;  // Prescaler value
    SSD1_TIMER->SR = 0;   // Clear pending IRQs
    SSD1_TIMER->DIER = 0; // DMA-related DIER bits
    SSD1_TIMER->PSC = 0;  // Prescaler value

    SSD1_TIMER->ARR = SSD1_TIMER_ARR;       // Time constant
    SSD1_TIMER->EGR = 0;                   // Update event
    SSD1_TIMER->CNT = 0;                   // Reset counter
    SSD1_TIMER->SR = 0;                    // Clear pending IRQs
    SSD1_TIMER->CR1 = STM32_TIM_CR1_CEN;   // Enable Timer
    SSD1_TIMER->DIER = STM32_TIM_DIER_UIE; // Update Event IRQ enabled
}

/*
 * INITIALIZATION
 */

void softSerialInit(void)
{
    palSetLineMode(SSD1_RX_LINE, PAL_MODE_INPUT_PULLUP);
    palSetLineMode(SSD1_TX_LINE, PAL_MODE_OUTPUT_PUSHPULL);
    ssdObjectInit(&SSD1);
    ssdStart(&SSD1, &ssd1_config);

    timerStart();
}

void softSerialStop(void)
{
    timerStop();

    ssdStop(&SSD1);
}

void keyboard_pre_init_user(void) {

    setPinOutputOpenDrain(C15);
    writePinHigh(C15);
    softSerialInit();
}

// uint8_t result = 0;


enum app_kbd_cmd {
    APP_CMD_KBD_NONE = 0x00,
    APP_CMD_KBD_SEND_KEY                   = 0x01,
    APP_CMD_KBD_PRESS_KEY                  = 0x02,
    APP_CMD_KBD_RELEASE_KEY                = 0x03,
    APP_CMD_KBD_SEND_MOUSE                 = 0x04,
    APP_CMD_KBD_GET_BATTERY                = 0x13,
};

// int8_t curent_cmd = APP_CMD_KBD_NONE;
uint8_t current_idx = 0;
// static uint8_t keyboard_cmd[1];
static uint8_t cmd[6];
static bool ready_to_send = false;
void housekeeping_task_user(void) {

    while (!iqIsEmptyI(&SSD1.iqueue)) {
        uint8_t res = ssdGetI(&SSD1);
        uint8_t current_cmd = cmd[0];
        if (current_cmd == APP_CMD_KBD_SEND_KEY && current_idx == 1) {
            uprintf("kbd %02x\n", res);
            tap_code(res);
            current_idx = 0;
            ready_to_send = true;
            break;
        } else if (current_cmd == APP_CMD_KBD_SEND_MOUSE && current_idx % 5 == 0) {
            uprintf("mouse %02x %d %d\n", cmd[1], cmd[2], cmd[3]);
            ready_to_send = true;
            current_idx = 0;
            break;
        } else {
            if (current_idx == 0) {
                uprintf("cmd %02x\n", res);
            } else {
                uprintf("data cmd:%02x idx:%d %02x\n",current_cmd, current_idx, res);
            }
            ready_to_send = false;
            cmd[current_idx] = res;
            current_idx++;
        }

    }
}

// pointing device
void pointing_device_driver_init(void) {

}

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    if (cmd[0] == APP_CMD_KBD_SEND_MOUSE && ready_to_send) {
        mouse_report.buttons = cmd[1];
        mouse_report.x = cmd[2];
        mouse_report.y = cmd[3];
        mouse_report.v = cmd[4];
        mouse_report.h = cmd[5];
        memset(cmd, 0, sizeof cmd);
        uprintf("pointer send\n");
    }
     return mouse_report;
}

uint16_t pointing_device_driver_get_cpi(void) {
    return 16;
}


void pointing_device_driver_set_cpi(uint16_t cpi) {

}

#endif

