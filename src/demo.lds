ENTRY(_start)

SECTIONS
{
    /* Start of TIM */
    . = 0x20000000;

    .text : {
        *(.text.init)
        *(.text.unlikely)
        *(.text.unlikely.*)
        *(.text.startup)
        *(.text.startup.*)
        *(.text)
        *(.text.*)
        *(.gnu.linkonce.t.*)
    }
    .rodata : {
        *(.rodata)
        *(.rodata.*)
        *(.gnu.linkonce.r.*)
    }

    .eh_frame : {
        *(.eh_frame)
    }

    .srodata : {
        *(.srodata.cst16)
        *(.srodata.cst8)
        *(.srodata.cst4)
        *(.srodata.cst2)
        *(.srodata.*)
    }

    .data : {
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
    }

    . = ALIGN(16);
    _sbss = .;
    .bss : {
        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
    }
    . = ALIGN(16);
    _ebss = .;
    
    . = . + 4095;
    . = ALIGN(4096);
    _stack_end = .;
}
