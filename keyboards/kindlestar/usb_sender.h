
#pragma once

#include "report.h"

void    usb_send_keyboard(report_keyboard_t *report);
void    usb_send_mouse(report_mouse_t *report);
void    usb_send_system(uint16_t data);
void    usb_send_consumer(uint16_t data);
void    usb_send_digitizer(report_digitizer_t *report);
