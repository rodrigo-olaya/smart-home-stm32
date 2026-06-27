    .syntax unified
    .cpu cortex-m4
    .thumb

    .section .isr_vector, "a"
    .word   _stack_top
    .word   Reset_Handler
    .word   0           /* NMI */
    .word   HardFault_Handler
    .word   0, 0, 0              /* MemManage, BusFault, UsageFault */
    .word   0, 0, 0, 0           /* reserved */
    .word   0, 0, 0, 0, 0        /* SVC, DebugMonitor, reserved, PendSV, SysTick */
    .word   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 /* Positions 0-27 */
    .word   tim2Handler
    .text
    .thumb_func
    .global Reset_Handler
Reset_Handler:
    ldr  r0, =_sidata
    ldr  r1, =_sdata
    ldr  r2, =_edata
copy_data:
    cmp  r1, r2
    bge  zero_bss
    ldr  r3, [r0], #4
    str  r3, [r1], #4
    b    copy_data

zero_bss:
    ldr  r0, =_sbss
    ldr  r1, =_ebss
    mov  r2, #0
zero_loop:
    cmp  r0, r1
    bge  call_main
    str  r2, [r0], #4
    b    zero_loop

call_main:
    bl   main
    b    .

    .thumb_func
    .global HardFault_Handler
HardFault_Handler:
    b    .
