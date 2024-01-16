
/* Copyright 2021 @ keystar (https://www.kindlestar.online)
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

#include "kindlestar.h"
#include "hal.h"
#include "gpio.h"
#include "config.h"
#include "kindlestar_output_auto_select.h"
#include "uart_sender.h"

#include "usb_main.h"
#include "usb_util.h"
#include "config.h"

#ifndef KEYSTAR_LED_RED_ON
#define KEYSTAR_LED_RED_ON() {}
#define KEYSTAR_LED_GREEN_ON() {}
#define KEYSTAR_LED_BLUE_ON() {}

#define KEYSTAR_LED_RED_OFF() {}
#define KEYSTAR_LED_GREEN_OFF() {}
#define KEYSTAR_LED_BLUE_OFF() {}
#endif

void check_standby(void);
void POWER_EnterSleep_v1(void);

void keyboard_pre_init_kb()
{
    // enable 4.2v power
    setPinOutput(B11);
    writePinLow(B11);

    // start timer for keyboard sleep
    start_sleep_timer();

    check_standby();

    orion_uart_init();
    init_send_mode();

    keyboard_pre_init_user();
}

// fixme
void reboot_system(void)
{

}

bool red_on = false;
void toggle_red(void)
{
    if (red_on)
    {
        KEYSTAR_LED_RED_OFF();
    } else {
        KEYSTAR_LED_RED_ON();
    }
    red_on = !red_on;
}


inline void perform_cmd(KBD_CMD cmd)
{
    sdPut(&SD1, cmd);
}

void perform_cmd_data(KBD_CMD cmd, uint8_t data) {
    sdPut(&SD1, cmd);
    sdPut(&SD1, data);
}

SEND_MODE mode = 0;
bool synced = false;
bool syncing = false;

void init_send_mode(void)
{
    mode = eeconfig_read_user();
    if (mode == SEND_MODE_USB)
    {
        ask_nrf_sleep();
    } else {
        wakeup_nrf();
        perform_cmd(mode);
    }
}

extern uint8_t sleep_counter;
void housekeeping_task_kb(void)
{
    housekeeping_task_user();
    if (!sdGetWouldBlock(&SD1))
    {
        // 数据同步完成
        uint8_t cmd = sdGet(&SD1);
        // sync success
        if (cmd == KBD_CMD_RESPONSE_SUCCESS)
        {
            synced = true;
            // KEYSTAR_LED_RED_OFF();
            // uprintf("----- response success-----\n");
            return;
        } else if (cmd == KBD_CMD_REQUEST_MODE) {
            synced = false;
            uprintf("----- request for send mode %d -----\n", (KBD_SELECT_CMD_USB + mode));
            perform_cmd(KBD_SELECT_CMD_USB + mode);
        } else if (cmd == KBD_CMD_REQUEST_CAPS_ON) {
            update_ble_keyboard_led_state(true);
            uprintf("update caps lock on\n");
        } else if (cmd == KBD_CMD_REQUEST_CAPS_OFF) {
            update_ble_keyboard_led_state(false);
            uprintf("update caps lock off\n");
        }
    }
    if (sleep_counter == 1) { // 持续使用
        chThdSleepMilliseconds(1);
    }
    else if (sleep_counter == 2) {
        chThdSleepMilliseconds(5);
    }
    else {
        chThdSleepMilliseconds(10);
    }
}

#ifdef NKRO_ENABLE
#    include "keycode_config.h"
extern keymap_config_t keymap_config;
#endif

#ifdef RGB_MATRIX_ENABLE
#include "rgb_matrix.h"
void rgb_matrix_reload_from_eeprom(void);
void finish_handler(void)
{
    // 重新读一下数据
    rgb_matrix_reload_from_eeprom();
    uprintf("indicate finished reload config %d %d %d %d", rgb_matrix_config.enable, rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v);
    if (rgb_matrix_is_enabled())
    {
        rgb_matrix_enable_noeeprom();
    } else {
        rgb_matrix_disable_noeeprom();
        DISABLE_POWER();
    }
}

void handlenkro(void);
#include "kindlestar_custom_animation.h"
void start_selection_rgb_animation(void)
{
    if (!rgb_matrix_is_enabled())
    {
        // 临时打开灯效
        rgb_matrix_enable_noeeprom();
    }

    ENABLE_POWER();
    rgblight_sethsv_noeeprom(0,0,0);
    // 从 g_led_config 找idx
    #ifdef START_BREATH_IDX
    start_breath_at_idx(START_BREATH_IDX - mode, finish_handler);
    #else
    start_breath_at_idx(32 - mode, finish_handler);
    #endif
}

void keyboard_post_init_kb(void) {
    if (mode != SEND_MODE_USB)
    {
#ifndef DEBUG
        // usb_device_state_set_suspend();
        usbStop(&USB_DRIVER);
        usbDisconnectBus(&USB_DRIVER);
#endif
    }

    start_selection_rgb_animation();
    handlenkro();
}

bool rgb_matrix_indicators_kb(void) {
    return custom_animation_task(0, 0);
}

#else
void start_selection_rgb_animation(void) {}
#endif

uint8_t fast_count = 0;
#define FAST_SYNC_INTERVAL 30
#define FAST_SYNC_TIME_MAX 200
static virtual_timer_t sync_vt;

static void sync_callback(virtual_timer_t *vtp, void *p)
{
    if (synced)
    {
        syncing = false;
        return;
    }
    // 1s快速同步结束后还不能完成同步数据，进入慢速扫描
    if (fast_count < FAST_SYNC_TIME_MAX)
    {
        fast_count++;
        chVTSetI(&sync_vt, TIME_MS2I(FAST_SYNC_INTERVAL), sync_callback, NULL);
    } else {
        chVTSetI(&sync_vt, TIME_S2I(1), sync_callback, NULL);
    }
    toggle_red();
}


static void start_sync(void)
{
    if (syncing)
    {
        return;
    }
    syncing = true;
    chVTObjectInit(&sync_vt);
    chVTSetI(&sync_vt, TIME_MS2I(FAST_SYNC_INTERVAL), sync_callback, NULL);
}

SEND_MODE current_send_mode(void)
{
    return mode;
}

void update_send_mode(SEND_MODE _mode)
{
    mode = _mode;
    eeconfig_update_user(mode);
    handlenkro();

    uprintf("update send mode to %d\n", mode);
}

void ask_nrf_sleep(void)
{
    // ble wake up io low, tell nrf to sleep
    // writePinLow(SLEEP_PIN);
}

void wakeup_nrf(void)
{
    // writePinHigh(SLEEP_PIN);
    start_sync();
}

void reboot_nrf(void)
{
    perform_cmd(KBD_CMD_REBOOT);
    start_sync();
}

void handlenkro(void)
{
#ifdef NKRO_ENABLE
    if ( mode == SEND_MODE_USB) {
        keymap_config.nkro = true;
        dprintf("keymap config changed 8");
    } else {
        keymap_config.nkro = false;
    }
#endif
}

void kbd_will_enter_sleep(void) {
    led_suspend();
    usbStop(&USB_DRIVER);
    usbDisconnectBus(&USB_DRIVER);
    // fixme reset usb status
    // setPinOutput(USBD_DP_PIN);
    // writePinLow(USBD_DP_PIN);
    sdStop(&SD1);
    // ble wake up io low, put nrf to sleep
    ask_nrf_sleep();
    // stop rgb light
    setPinInput(C14);

    shutdown_user();
    // setPinInputHigh(A0);
    // palEnableLineEvent(A0, PAL_EVENT_MODE_BOTH_EDGES);

}


void enter_standby_mode(void)
{
    kbd_will_enter_sleep();
    POWER_EnterSleep_v1();
        // POWER_EnterSleep();
}

// 默认使用这个
#ifdef QMK_MCU_STM32F103
void bootloader_jump(void)
{
    BKP->DR10 = 0x424C;
    wait_ms(10);
    NVIC_SystemReset();
}

#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)
#define PWR_CR_OFFSET            0x00U
#define PWR_CSR_OFFSET           0x04U
#define PWR_CR_OFFSET_BB         (PWR_OFFSET + PWR_CR_OFFSET)
#define PWR_CSR_OFFSET_BB        (PWR_OFFSET + PWR_CSR_OFFSET)
#define CSR_EWUP_BB(VAL)         ((uint32_t)(PERIPH_BB_BASE + (PWR_CSR_OFFSET_BB * 32U) + (POSITION_VAL(VAL) * 4U)))
void check_standby(void)
{
    if (PWR->CSR & PWR_CSR_SBF) {
        PWR->CR |= PWR_CR_CWUF;
        PWR->CR |= PWR_CR_CSBF;
          *(__IO uint32_t *) CSR_EWUP_BB(PWR_CSR_EWUP) = (uint32_t)DISABLE;
        // HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1); // for hal
    }
}


////////////////////////// stanby mode ///////////////////////////////////

// static void POWER_EnterSleep(void) {
//     /* Clear Wake-up flag */
//     PWR->CR |= PWR_CR_CWUF | PWR_CR_CSBF;
//     /* Select Sleep mode */
//     /* PWR->CR |= PWR_CR_PDDS | PWR_CR_LPDS; */
//     /* Set SLEEPDEEP bit of Cortex System Control Register */
//     SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
//     /* Request Wait For Interrupt */
//     __WFI();
//     /* Reset SLEEPDEEP bit of Cortex System Control Register */
//     SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
//     NVIC_SystemReset();
// }

void POWER_EnterSleep_v1(void) {
#if(DEBUG_ENABLE)
// DBGMCU->CR |= DBGMCU_CR_DBG_STOP | DBGMCU_CR_DBG_STANDBY;
#endif
    // https://www.youtube.com/watch?v=O82rj9qxkgs
    // enable pwr control clock
    RCC->APB1ENR |= (RCC_APB1ENR_PWREN);

    // #if (!defined(DEBUG) || !defined(USE_DBG_STANDBY))
    // /* Disable DBG_STANDBY. Prevent DBG_STANDBY from being enabled by debugger when
    //  * downloading programs, causing standby mode power consumption to be too high */
    // SET_BIT(RCC->APB2ENR, RCC_APB2ENR_DBGMCUEN);
    // CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
    // #endif


    // // set sleepdeep mask
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    // // power down deep sleep,  1 is standby mode
    PWR->CR |= PWR_CR_PDDS;

    PWR->CSR |= PWR_CSR_EWUP;
    PWR->CR |= PWR_CR_CWUF;

    __WFI();
}
#endif
