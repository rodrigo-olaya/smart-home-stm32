CC = arm-none-eabi-gcc

CFLAGS = -mcpu=cortex-m4 \
-mthumb \
-ffreestanding \
-nostdlib \
-nostartfiles \
-T linker/stm32f411ce.ld \
-g \

firmware.elf: src/main.c startup/startup_stm32f407xx.s src/drivers/uart/uart.c src/drivers/timers/timer.c src/drivers/gpio/gpio.c src/drivers/sonar/sonar.c src/drivers/exti/exti.c src/drivers/pwm/pwm.c src/heartbeatHandler/heartbeatHandler.c src/positionHandler/positionHandler.c
	$(CC) $(CFLAGS) src/main.c startup/startup_stm32f407xx.s src/drivers/uart/uart.c src/drivers/timers/timer.c src/drivers/gpio/gpio.c src/drivers/sonar/sonar.c src/drivers/exti/exti.c src/drivers/pwm/pwm.c src/heartbeatHandler/heartbeatHandler.c src/positionHandler/positionHandler.c -o firmware.elf

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary firmware.elf firmware.bin

# USB DFU, no ST-Link needed. First put the pill in bootloader mode:
# hold BOOT0, tap NRST, release BOOT0.
flash-dfu: firmware.bin
	dfu-util -a 0 -s 0x08000000:leave -D firmware.bin

clean:
	rm -f firmware.elf firmware.bin