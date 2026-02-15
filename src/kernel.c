#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "alloc.h"
#include "asm.h"
#include "boot.h"
#include "cpu.h"
#include "gdt.h"
#include "idt.h"
#include "panic.h"
#include "print.h"
#include "slice.h"
#include "str.h"
#include "terminal.h"

extern char stack_bottom, stack_top;

void print_info(void) {
  char buf[100];
  char* head = buf;
  head = strcpy(head, "stack ");
  head = itohex((uint32_t)&stack_top, SLICE(head, 13));
  head = strcpy(head, " to ");
  head = itohex((uint32_t)&stack_bottom, SLICE(head, 13));
  head = strcpy(head, " with size ");
  head =
      itohex((uint32_t)&stack_top - (uint32_t)&stack_bottom, SLICE(head, 13));
  term_writeline(buf);
}

void kernel_main(uint32_t bootloader_magic, void* info) {
  term_init();
  term_writeline("bad operating system version " __VERSION " booting...");

  if (!verify_bootloader_signature(bootloader_magic))
    panic("invalid bootloader magic number, only multiboot2 is supported");

  struct bootloader_info bl_info = bootloader_info_load(info);

  term_write("loaded by ");
  if (bl_info.bootloader_name)
    term_write(bl_info.bootloader_name);
  else
    term_write("unknown bootloader");

  term_write(" with args ");
  if (bl_info.args)
    term_write(bl_info.args);
  else
    term_write("N/A");
  term_writeline("");

  print_info();

  if (is_protected_mode())
    term_writeline("running in 32-bit protected mode");
  else
    panic("uh-oh, we are running in 16-bit real mode...");

  term_write("running on ");
  term_writeline(get_vendor_id().name);

  setup_gdt();
  setup_idt();

  term_writeline("exiting...");
}
