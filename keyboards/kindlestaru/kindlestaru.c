
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

#include "kindlestaru.h"
#include "hal.h"
#include "gpio.h"
#include "config.h"

#include "usb_main.h"
#include "usb_util.h"
#include "config.h"


void check_standby(void);
void POWER_EnterSleep_v1(void);

void keyboard_pre_init_user()
{
    // enable 4.2v power
    setPinOutput(B11);
    writePinLow(B11);

    // start timer for keyboard sleep

    check_standby();

}

// fixme
void reboot_system(void)
{

}

#ifdef NKRO_ENABLE
#    include "keycode_config.h"
extern keymap_config_t keymap_config;
#endif

void start_selection_rgb_animation(void) {}


void handlenkro(void)
{
    keymap_config.nkro = true;
}

void kbd_will_enter_sleep(void) {
    led_suspend();
    usbStop(&USB_DRIVER);
    usbDisconnectBus(&USB_DRIVER);
    shutdown_user();

}


void enter_standby_mode(void)
{
    kbd_will_enter_sleep();
    POWER_EnterSleep_v1();
}