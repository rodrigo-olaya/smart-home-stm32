CC = arm-none-eabi-gcc

CFLAGS = -mcpu=cortex-m4 \
-mthumb \
-ffreestanding \
-nostdlib \
-nostartfiles \
-T linker/stm32f407vg.ld \
-g

firmware.elf: src/main.c startup/startup_stm32f407xx.s
	$(CC) $(CFLAGS) src/main.c startup/startup_stm32f407xx.s -o firmware.elf

clean:
	rm -f firmware.elf