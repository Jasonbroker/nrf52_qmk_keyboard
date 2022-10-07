# MCU name
MCU   = GD32VF103
BOARD = SIPEED_LONGAN_NANO

# Bootloader selection
BOOTLOADER = gd32v-dfu

# Build Options
#   change yes to no to disable
#
KEYBOARD_SHARED_EP = yes

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no       # Virtual DIP switch configuration
MOUSEKEY_ENABLE = yes          # Mouse keys
EXTRAKEY_ENABLE = yes         # Audio control and System control
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no         # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = no             # USB Nkey Rollover fixme
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no         # Enable keyboard RGB underglow
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812
AUDIO_ENABLE = no           # Audio output

ENCODER_ENABLE = yes

OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
DEBOUNCE_TYPE = sym_defer_pk    # Debounce using eager_pk.
NO_USB_STARTUP_CHECK = yes
# CUSTOM_MATRIX = lite
# DEFERRED_EXEC_ENABLE = yes


# debug info
DEBUG_ENABLE :=no
CONSOLE_ENABLE :=no
# COMMAND_ENABLE = yes           # Commands for debug and configuration

#EXCLUDE = $(CHIBIOS_DIR)
SRC += ../kindlestar_output_auto_select.c
SRC += ../usb_sender.c
SRC += ../uart_sender.c
SRC += ../kindlestar_custom_animation.c

OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

KINDLE_PATH = keyboards/kindlestar
# OPT_DEFS += -DUSE_HAL_DRIVER

# OPT_DEFS += -DSTM32F103xB  # duino 里面定义了已经
# OPT_DEFS += -DSTM32_LSECLK=0
