
#pragma once

#include "report.h"

void orion_uart_init(void);

void    orion_uart_send_keyboard(report_keyboard_t *report);
void    orion_uart_send_mouse(report_mouse_t *report);
void    orion_uart_send_system(uint16_t data);
void    orion_uart_send_consumer(uint16_t data);
void    orion_uart_send_digitizer(report_digitizer_t *report);
