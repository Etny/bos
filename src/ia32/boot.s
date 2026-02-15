

.section .bss
.globl stack_bottom
.globl stack_top
.align 16
stack_bottom:
.skip 16384
stack_top:


.section .text
.global _start
.type _start, @function
_start:
    // Set up stack
    mov $0, %ebp
    mov $stack_top, %esp

    // Move the bootloader information
    // into the right registers to pass
    // to kernel main
    push %ebx
    push %eax
    call kernel_main

    cli
1:  hlt
    jmp 1b


.size _start, . - _start
