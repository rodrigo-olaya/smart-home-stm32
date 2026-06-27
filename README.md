# smart-home-stm32
Smart Home on STM32F4 board.

## Build and flash
```
make clean && make
openocd -f board/stm32f4discovery.cfg -c "program firmware.elf verify reset exit"
```