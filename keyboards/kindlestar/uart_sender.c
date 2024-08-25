#include "uart_sender.h"
#include "hal.h"
#include "gpio.h"
#include "print.h"
#ifdef NKRO_ENABLE
#    include "keycode_config.h"
extern keymap_config_t keymap_config;
#endif
#include "kindlestar.h"

// https://www.playembedded.org/blog/stm32-usart-chibios-serial/
const SerialConfig config = {
    115200,
    0,
    USART_CR2_STOP1_BITS,
    0
    };

void orion_uart_init(void)
{
  sdStart(&SD1, &config);
#ifndef QMK_MCU_STM32F103
  palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(1));       /* USART1 TX.       */
  palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(1));      /* USART1 RX. */
#endif
}

extern SEND_MODE mode;
void orion_uart_send_keyboard(report_keyboard_t *report)
{
#ifdef NKRO_ENABLE
if (keymap_config.nkro)
{
    uprintf("canot enter this place bits %d mod %x\n", KEYBOARD_REPORT_BITS, report->nkro.mods);
    keymap_config.nkro = false;
} else
#endif
{
    uint8_t *raw = report->keys;
    sdPut(&SD1, KBD_SEND_CMD_REPORT_ID_KEYBOARD | mode);
    sdWrite(&SD1, raw, KEYBOARD_REPORT_SIZE);
}

}

void orion_uart_send_mouse(report_mouse_t *report)
{
    sdPut(&SD1, KBD_SEND_CMD_REPORT_ID_MOUSE | mode);
    sdPut(&SD1, report->buttons);
    sdPut(&SD1, report->x);
    sdPut(&SD1, report->y);
    sdPut(&SD1, report->v);
    sdPut(&SD1, report->h);
}

void orion_uart_send_system(uint16_t data)
{
    sdPut(&SD1, KBD_SEND_CMD_REPORT_ID_SYSTEM | mode);
    sdPut(&SD1, data >> 8);
    sdPut(&SD1, data);
}

void orion_uart_send_consumer(uint16_t data)
{
    sdPut(&SD1, KBD_SEND_CMD_REPORT_ID_CONSUMER | mode);
    sdPut(&SD1, data >> 8);
    sdPut(&SD1, data);
}

void orion_uart_send_digitizer(report_digitizer_t *report)
{

}
