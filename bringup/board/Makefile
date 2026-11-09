CC = arm-none-eabi-gcc

CFLAGS = -mcpu=cortex-m4 \
-mthumb \
-ffreestanding \
-nostdlib \
-nostartfiles \
-T stm32f411ce.ld \
-g

board.elf: main.c startup.s stm32f411ce.ld
	$(CC) $(CFLAGS) main.c startup.s -o board.elf

# ST-Link V2 wired to the Black Pill's SWD header (3V3, GND, SWDIO, SWCLK)
flash: board.elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program board.elf verify reset exit"

board.bin: board.elf
	arm-none-eabi-objcopy -O binary board.elf board.bin

# USB DFU, no ST-Link needed. First put the pill in bootloader mode:
# hold BOOT0, tap NRST, release BOOT0 (or hold BOOT0 while plugging in USB).
flash-dfu: board.bin
	dfu-util -a 0 -s 0x08000000:leave -D board.bin

clean:
	rm -f board.elf board.bin
