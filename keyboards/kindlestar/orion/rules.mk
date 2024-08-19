# MCU name
MCU = STM32F103

BOOTLOADER = stm32duino

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

# debug info
DEBUG_ENABLE :=no
CONSOLE_ENABLE :=no
# COMMAND_ENABLE = yes           # Commands for debug and configuration

#CHIBIOS_DIR = ../../../tmk_core/protocol/chibios
#CHIBIOS_DIR = /Users/zzc/Documents/keyboard/referals/qmk_firmware/tmk_core/protocol/chibios
#old_usb_main = $(CHIBIOS_DIR)/usb_main.c
#EXCLUDE = $(CHIBIOS_DIR)
SRC += ../kindlestar_output_auto_select.c
SRC += ../usb_sender.c
SRC += ../uart_sender.c
SRC += ../kindlestar_custom_animation.c

OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

KINDLE_PATH = keyboards/kindlestar
HAL = Drivers/STM32F1xx_HAL_Driver
# COMMON_VPATH += $(HAL)
VPATH += $(KINDLE_PATH)/$(HAL)/Inc
VPATH += $(KINDLE_PATH)/Drivers
SRC += $(HAL)/Src/stm32f1xx_hal.c
SRC += $(HAL)/Src/stm32f1xx_hal_pwr.c
SRC += $(HAL)/Src/stm32f1xx_hal_gpio.c
SRC += $(HAL)/Src/stm32f1xx_hal_cortex.c
# OPT_DEFS += -DUSE_HAL_DRIVER

OPT_DEFS += -DSTM32F103xB  # duino 里面定义了已经
OPT_DEFS += -DSTM32_LSECLK=0
