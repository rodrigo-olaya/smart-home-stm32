CC = arm-none-eabi-gcc

CFLAGS = -mcpu=cortex-m4 \
-mthumb \
-ffreestanding \
-nostdlib \
-nostartfiles \
-T stm32f411ce.ld \
-g

gm65.elf: main.c startup.s stm32f411ce.ld
	$(CC) $(CFLAGS) main.c startup.s -o gm65.elf

# ST-Link V2 wired to the Black Pill's SWD header (3V3, GND, SWDIO, SWCLK)
flash: gm65.elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program gm65.elf verify reset exit"

gm65.bin: gm65.elf
	arm-none-eabi-objcopy -O binary gm65.elf gm65.bin

# USB DFU, no ST-Link needed. First put the pill in bootloader mode:
# hold BOOT0, tap NRST, release BOOT0 (or hold BOOT0 while plugging in USB).
flash-dfu: gm65.bin
	dfu-util -a 0 -s 0x08000000:leave -D gm65.bin

clean:
	rm -f gm65.elf gm65.bin
