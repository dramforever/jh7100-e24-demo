    .global _start
    .section .text.init
_start:
    la sp, _stack_end
    la t0, _sbss
    la t1, _ebss

1:
    beq t0, t1, .jump_entry
    sw zero, (t0)
    addi t0, t0, 4
    j 1b

    la t0, trap_handler
    csrw mtvec, t0

.jump_entry:
    call main

1:
    wfi
    j 1b

trap_handler:
    j trap_handler
