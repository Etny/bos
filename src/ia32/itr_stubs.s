
.section .text

.altmacro

.macro ITR_STUB_NONERROR num
itr_stub_\num:
    pushl $0
    pushl $\num
    jmp itr_stub_common
.endm

.macro ITR_STUB_ERROR num
itr_stub_\num:
    pushl $\num
    jmp itr_stub_common
.endm

ITR_STUB_NONERROR 0
ITR_STUB_NONERROR 1
ITR_STUB_NONERROR 2
ITR_STUB_NONERROR 3
ITR_STUB_NONERROR 4
ITR_STUB_NONERROR 5
ITR_STUB_NONERROR 6
ITR_STUB_NONERROR 7
ITR_STUB_ERROR 8
ITR_STUB_NONERROR 9
ITR_STUB_ERROR 10
ITR_STUB_ERROR 11
ITR_STUB_ERROR 12
ITR_STUB_ERROR 13
ITR_STUB_ERROR 14
ITR_STUB_NONERROR 15
ITR_STUB_NONERROR 16
ITR_STUB_ERROR 17
ITR_STUB_NONERROR 18
ITR_STUB_NONERROR 19
ITR_STUB_NONERROR 20
ITR_STUB_ERROR 21

.extern exception_handler
itr_stub_common:
    pusha 

    mov %ds, %ax     # move ds into the lower 16 bits of eax
    pushl %eax        # and push it to reuse later

    mov $0x10, %ax     # load the kernel data segment selector
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    push %esp
    call exception_handler
    add $4, %esp 

    popl %ebx         # return ds
    mov %bx, %ds
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs
    mov %bx, %ss

    popa
    addl $8, %esp     # move the stack down 8 bytes to clean 
    iret            # up the pushed error- and int code

.section .data

# needed to properly expand the macro below
.macro ITR_STUB_LABEL i
    .long itr_stub_\i
.endm

.globl itr_stub_table
itr_stub_table:
    .set i, 0

    .rept 22
        ITR_STUB_LABEL %i
        .set i, i+1
    .endr



