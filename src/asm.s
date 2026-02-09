

.section .text
.globl enable_protected_mode
.type enable_protected_mode, @function

enable_protected_mode:
    movl    %cr0, %eax
    orb     $0, %al
    movl    %eax, %cr0

    ljmp $0x08, $1f
    1:
    ret

.section .text
.globl refresh_cs
.type refresh_cs, @function

refresh_cs:
    ljmp $0x08, $1f
    1:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret

