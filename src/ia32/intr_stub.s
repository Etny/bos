
.section .text
.extern except_base

.altmacro

.macro INTR_STUB num
intr_stub_\num:
    call except_base
    iret
.endm

.set i, 0

.rept 32
    INTR_STUB %i
    .set i, i+1
.endr

.macro INTR_STUB_LABEL i
    .long intr_stub_\i
.endm

.section .data
.globl intr_stub_table
intr_stub_table:
    .set i, 0

    .rept 32
        INTR_STUB_LABEL %i
        .set i, i+1
    .endr



