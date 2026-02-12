#ifndef __GDT_DEC_H
#define __GDT_DEC_H

#define GDT_NULL_SEG        0
#define GDT_KERNEL_CODE_SEG 1
#define GDT_KERNEL_DATA_SEG 2

#define GDT_KERNEL_CODE_SEG_IDX 8

#define TABLE_GDT 0
#define TABLE_IDT 1

#define SEG_SELECTOR(rpl, table, index) (0 | (rpl & 3) | ((index & 1) << 2) | (index & 0xFFF8))

#endif  // !__GDT_DEC_H
