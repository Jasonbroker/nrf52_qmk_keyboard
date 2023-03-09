#include "kindlestar_output_auto_select.h"
#include "usb_sender.h"
#include "uart_sender.h"
#include "kindlestar.h"
#include "usb_main.h"

static virtual_timer_t sleep_vt;
uint8_t sleep_counter = 1;
#define SLEEP_OFF_TIMEOUT 30 * 2 // power off time , then shutdown     30min
#define SLEEP_CHECK_INTERVAL 30 // 一共300s 5min

static inline void keyboard_sleep_counter_reset(void) {
    sleep_counter = 1;
}

#include "usb_device_state.h"
/*
static can_use_usb = true;
void notify_usb_device_state_change_kb(enum usb_device_state usb_device_state);
{

}
*/

static void sleep_cb(virtual_timer_t *vtp, void *p)
{
    uprintf("sleep count time %d \n", sleep_counter);
    if (sleep_counter >= SLEEP_OFF_TIMEOUT) {
        uprintf("will enter sleep mode \n");
        enter_standby_mode();
    }
    else {
        sleep_counter++;
        if (current_send_mode() != SEND_MODE_USB){
            sleep_counter++;
        }
        chVTSetI(&sleep_vt, TIME_S2I(SLEEP_CHECK_INTERVAL), sleep_cb, NULL);
    }
}

void start_sleep_timer(void)
{
    /* LED timer initialization.*/
    chVTObjectInit(&sleep_vt);
    /* Starting blinker.*/
    chVTSetI(&sleep_vt, TIME_S2I(SLEEP_CHECK_INTERVAL), sleep_cb, NULL);
}

uint8_t ble_keyboard_led_state;
void update_ble_keyboard_led_state(bool on)
{
    if (on) {
        ble_keyboard_led_state |= 1 << USB_LED_CAPS_LOCK;
    } else {
        ble_keyboard_led_state = 0;
    }
}

void    send_keyboard(report_keyboard_t *report);
void    send_mouse(report_mouse_t *report);
void    send_extra(report_extra_t *report);

uint8_t orion_keyboard_leds(void);
void    orion_send_keyboard(report_keyboard_t *report);
void    orion_send_mouse(report_mouse_t *report);
void    orion_send_extra(report_extra_t *report);

host_driver_t orion_chibios_driver = {orion_keyboard_leds, orion_send_keyboard, orion_send_mouse, orion_send_extra};
//
extern uint8_t keyboard_led_state;

uint8_t orion_keyboard_leds(void)
{
    return current_send_mode() == SEND_MODE_USB ? keyboard_led_state : ble_keyboard_led_state;
}

void orion_send_keyboard(report_keyboard_t *report)
{
    uprintf("send using mode %d\n", current_send_mode());
    if (current_send_mode() == SEND_MODE_USB){
        if (usb_device_state != USB_DEVICE_STATE_SUSPEND)
        {
            send_keyboard(report);
        }
    } else {
        orion_uart_send_keyboard(report);
    }
    keyboard_sleep_counter_reset();
}

void orion_send_mouse(report_mouse_t *report)
 {
     uprintf("send mouse using mode %d\n", current_send_mode());
     if (current_send_mode() == SEND_MODE_USB)
     {
        if (usb_device_state != USB_DEVICE_STATE_SUSPEND)
        {
        send_mouse(report);
        }
     } else {
        dprintf("send using uart");
        orion_uart_send_mouse(report);
    }
}


void    orion_send_extra(report_extra_t *report)
{
    uprintf("send mouse using mode %d\n", current_send_mode());
    if (current_send_mode() == SEND_MODE_USB)
    {
        if (usb_device_state != USB_DEVICE_STATE_SUSPEND)
        {
            send_extra(report);
        }
    } else {
        if (report->report_id == REPORT_ID_SYSTEM) {
            orion_uart_send_system(report->usage);
        } else if (report->report_id == REPORT_ID_CONSUMER) {
            orion_uart_send_consumer(report->usage);
        }
    }
}
