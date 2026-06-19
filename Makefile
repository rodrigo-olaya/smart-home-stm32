CC = arm-none-eabi-gcc

CFLAGS = -mcpu=cortex-m4 \
-mthumb \
-ffreestanding \
-nostdlib \
-nostartfiles \
-T linker/stm32f407vg.ld \
-g

firmware.elf: src/main.c startup/startup_stm32f407xx.s src/drivers/uart/uart.c
	$(CC) $(CFLAGS) src/main.c startup/startup_stm32f407xx.s src/drivers/uart/uart.c -o firmware.elf

clean:
	rm -f firmware.elf