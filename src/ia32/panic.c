#include "panic.h"

#include <stdint.h>

#include "multiboot2.h"
#include "print.h"
#include "slice.h"
#include "str.h"
#include "terminal.h"

void print_source_line(uint32_t ret_addr) {
  uint32_t largest_entry = 0;
  const char *name = 0;
  for (size_t idx = 1; idx < loaded_symbols.entries; idx++) {
    if (ELF32_ST_TYPE(loaded_symbols.symtab[idx].st_info) != ELF32_TYPE_FUNC)
        && ELF32_ST_TYPE(loaded_symbols.symtab[idx].st_info) != ELF32_TYPE_LABEL)) continue;
    if (loaded_symbols.symtab[idx].st_value > ret_addr - 1) continue;
    if (loaded_symbols.symtab[idx].st_value < largest_entry) continue;
    largest_entry = loaded_symbols.symtab[idx].st_value;
    name = loaded_symbols.strtab + loaded_symbols.symtab[idx].st_name;
  }
  char hex_buf[12];
  itohex(ret_addr - largest_entry, TO_SLICE(hex_buf));
  term_write("    ");
  term_write(name);
  term_write(" + ");
  term_write(hex_buf);
  term_write(" @ ");
  itohex(ret_addr, TO_SLICE(hex_buf));
  term_writeline(hex_buf);
}

struct stack_frame {
  struct stack_frame *next;
  uintptr_t ret_addr;
};

void print_stack(void) {
  struct stack_frame *stack;
  asm volatile("mov %%ebp, %0" : "=r"(stack));

  while (stack) {
    print_source_line(stack->ret_addr);
    stack = stack->next;
  }
}

void print_stack_from(uint32_t ip, uint32_t bp) {
  print_source_line(ip);
  struct stack_frame *stack = (struct stack_frame *)bp;

  while (stack) {
    print_source_line(stack->ret_addr);
    stack = stack->next;
  }
}

void _panic(const char *error, const char *file, const char *line,
            struct panic_context ctx) {
  asm volatile("cli");
  term_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
  term_writeline("--!PANIC!--");
  term_write("At ");
  term_write(file);
  term_write(":");
  term_writeline(line);
  term_writeline(error);
  term_writeline("stacktrace:");

  if (ctx.ip == 0)
    print_stack();
  else
    print_stack_from(ctx.ip, ctx.bp);

  for (;;);
}
