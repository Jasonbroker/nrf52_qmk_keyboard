
UART_DRIVER_REQUIRED = yes

OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
DEBOUNCE_TYPE = sym_defer_pk    # Debounce using eager_pk.
NO_USB_STARTUP_CHECK = yes

#EXCLUDE = $(CHIBIOS_DIR)
SRC += ../kindlestar_output_auto_select.c
# SRC += ../usb_sender.c
SRC += ../uart_sender.c
SRC += ../kindlestar_custom_animation.c


OPT_DEFS += -DWS2812_BITBANG_NOP_FUDGE=1
