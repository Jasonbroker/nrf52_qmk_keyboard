
# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes       # Virtual DIP switch configuration
MOUSEKEY_ENABLE = yes          # Mouse keys
EXTRAKEY_ENABLE = yes         # Audio control and System control
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no         # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = yes             # USB Nkey Rollover
BACKLIGHT_ENABLE = yes       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no         # Enable keyboard RGB underglow
BACKLIGHT_DRIVER ?= pwm
# BACKLIGHT_DRIVER ?= software
# RGB_MATRIX_ENABLE = yes
# RGB_MATRIX_DRIVER = WS2812
AUDIO_ENABLE = no           # Audio output

# ENCODER_ENABLE = yes

OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
DEBOUNCE_TYPE = sym_defer_pk    # Debounce using eager_pk.
# DEBOUNCE_TYPE = sym_eager_pr
NO_USB_STARTUP_CHECK = yes
# CUSTOM_MATRIX = lite
# DEFERRED_EXEC_ENABLE = yes


# debug info
DEBUG_ENABLE ?= no
CONSOLE_ENABLE ?= no
# COMMAND_ENABLE = yes           # Commands for debug and configuration

OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
OPT_DEFS += -DUSE_USB_MOD=1

KINDLE_PATH = keyboards/kindlestar
# OPT_DEFS += -DUSE_HAL_DRIVER

# OPT_DEFS += -DSTM32F103xB  # duino 里面定义了已经
# OPT_DEFS += -DSTM32_LSECLK=0
