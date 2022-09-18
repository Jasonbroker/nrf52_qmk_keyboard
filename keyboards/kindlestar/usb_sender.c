#include "usb_sender.h"
#include <hal.h>
#include <string.h>
#include "host.h"
#include "debug.h"
#include "suspend.h"
#ifdef SLEEP_LED_ENABLE
#    include "sleep_led.h"
#    include "led.h"
#endif
#include "wait.h"
#include "usb_descriptor.h"
#include "usb_driver.h"
#include "usb_main.h"

#ifdef NKRO_ENABLE
#    include "keycode_config.h"

extern keymap_config_t keymap_config;
#endif

#ifdef JOYSTICK_ENABLE
#    include "joystick.h"
#endif

extern report_keyboard_t keyboard_report_sent;
void usb_send_keyboard(report_keyboard_t *report)
{
    osalSysLock();
    if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
        goto unlock;
    }

#ifdef NKRO_ENABLE
    if (keymap_config.nkro && keyboard_protocol) { /* NKRO protocol */
        /* need to wait until the previous packet has made it through */
        /* can rewrite this using the synchronous API, then would wait
         * until *after* the packet has been transmitted. I think
         * this is more efficient */
        /* busy wait, should be short and not very common */
        if (usbGetTransmitStatusI(&USB_DRIVER, SHARED_IN_EPNUM)) {
            /* Need to either suspend, or loop and call unlock/lock during
             * every iteration - otherwise the system will remain locked,
             * no interrupts served, so USB not going through as well.
             * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
            osalThreadSuspendS(&(&USB_DRIVER)->epc[SHARED_IN_EPNUM]->in_state->thread);

            /* after osalThreadSuspendS returns USB status might have changed */
            if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
                goto unlock;
            }
        }
        usbStartTransmitI(&USB_DRIVER, SHARED_IN_EPNUM, (uint8_t *)report, sizeof(struct nkro_report));
    } else
#endif /* NKRO_ENABLE */
    {  /* regular protocol */
        /* need to wait until the previous packet has made it through */
        /* busy wait, should be short and not very common */
        if (usbGetTransmitStatusI(&USB_DRIVER, KEYBOARD_IN_EPNUM)) {
            /* Need to either suspend, or loop and call unlock/lock during
             * every iteration - otherwise the system will remain locked,
             * no interrupts served, so USB not going through as well.
             * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
            osalThreadSuspendS(&(&USB_DRIVER)->epc[KEYBOARD_IN_EPNUM]->in_state->thread);

            /* after osalThreadSuspendS returns USB status might have changed */
            if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
                goto unlock;
            }
        }
        uint8_t *data, size;
        if (keyboard_protocol) {
            data = (uint8_t *)report;
            size = KEYBOARD_REPORT_SIZE;
        } else { /* boot protocol */
            data = &report->mods;
            size = 8;
        }
        usbStartTransmitI(&USB_DRIVER, KEYBOARD_IN_EPNUM, data, size);
    }
    keyboard_report_sent = *report;

unlock:
    osalSysUnlock();
}

#ifdef MOUSE_ENABLE
void usb_send_mouse(report_mouse_t *report)
{
    osalSysLock();
    if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
        osalSysUnlock();
        return;
    }

    if (usbGetTransmitStatusI(&USB_DRIVER, MOUSE_IN_EPNUM)) {
        /* Need to either suspend, or loop and call unlock/lock during
         * every iteration - otherwise the system will remain locked,
         * no interrupts served, so USB not going through as well.
         * Note: for suspend, need USB_USE_WAIT == TRUE in halconf.h */
        if (osalThreadSuspendTimeoutS(&(&USB_DRIVER)->epc[MOUSE_IN_EPNUM]->in_state->thread, TIME_MS2I(10)) == MSG_TIMEOUT) {
            osalSysUnlock();
            return;
        }
    }
    usbStartTransmitI(&USB_DRIVER, MOUSE_IN_EPNUM, (uint8_t *)report, sizeof(report_mouse_t));
    osalSysUnlock();
}
#else
void usb_send_mouse(report_mouse_t *report) {};
#endif

#ifdef EXTRAKEY_ENABLE
void send_extra(uint8_t report_id, uint16_t data);
#endif

void usb_send_system(uint16_t data)
{
#ifdef EXTRAKEY_ENABLE
    send_extra(REPORT_ID_SYSTEM, data);
#endif
}
void usb_send_consumer(uint16_t data)
{
#ifdef EXTRAKEY_ENABLE
    send_extra(REPORT_ID_CONSUMER, data);
#endif
}
void usb_send_digitizer(report_digitizer_t *report)
{
#ifdef DIGITIZER_ENABLE
#    ifdef DIGITIZER_SHARED_EP
    osalSysLock();
    if (usbGetDriverStateI(&USB_DRIVER) != USB_ACTIVE) {
        osalSysUnlock();
        return;
    }

    usbStartTransmitI(&USB_DRIVER, DIGITIZER_IN_EPNUM, (uint8_t *)report, sizeof(report_digitizer_t));
    osalSysUnlock();
#    else
    chnWrite(&drivers.digitizer_driver.driver, (uint8_t *)report, sizeof(report_digitizer_t));
#    endif
#endif
}
