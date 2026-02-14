
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


.section .text
.globl read_vendor_id
.type read_vendor_id, @function
read_vendor_id:
    push %ebp
    mov %esp, %ebp
    push %ebx
    mov 8(%ebp), %edi
    xor %eax, %eax
    cpuid
    mov %ebx, 0(%edi)
    mov %edx, 4(%edi)
    mov %ecx, 8(%edi)
    movb $0, 12(%edi)
    pop %ebx
    pop %ebp
    ret

