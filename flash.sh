#! /bin/bash

#build
make kindlestar/aquila

#target=./build/kindlestar_orion_rev1_via.hex

# https://wiki.segger.com/J-Link_Commander
# call JLink.exe -JLinkScriptFile ${path to script file for Device A} -device STM32F103C8 -if SWD -speed 4000 -autoconnect 1 -CommandFile ${path to command file for Device A}
JLinkExe -device STM32F103CB -if SWD -speed 4000 -autoconnect 1 -NoGui 1 -CommandFile flash_stm32.txt

exit

# mergehex
# mergehex -m .build/keystar_apm32f103_boot20_pc13_fastboot.bin .build/kindlestar_orion_rev1_via.hex -o kindlestar_keyboard_merge_via_bl.hex
