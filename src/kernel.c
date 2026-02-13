#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "alloc.h"
#include "asm.h"
#include "cpu.h"
#include "gdt.h"
#include "idt.h"
#include "panic.h"
#include "str.h"
#include "terminal.h"

char buf[20];

void kernel_main(void) {
  term_init();
  term_writeline("Bad Operating System version " __VERSION " booting...");

  char buf[20];
  read_vendor_id(buf);
  term_write("running on ");
  term_writeline(buf);

  bool is_protected = is_protected_mode();

  if (is_protected)
    term_writeline("running in 32-bit protected mode");
  else
    panic("Uh-oh, we are running in 16-bit real mode...");

  setup_gdt();
  setup_idt();

  term_writeline("exiting...");
}
