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
#include "print.h"
#include "slice.h"
#include "str.h"
#include "terminal.h"

void kernel_main(void) {
  term_init();
  term_writeline("Bad Operating System version " __VERSION " booting...");

  if (is_protected_mode())
    term_writeline("running in 32-bit protected mode");
  else
    panic("Uh-oh, we are running in 16-bit real mode...");

  term_write("running on ");
  term_writeline(get_vendor_id().name);

  setup_gdt();
  setup_idt();

  term_writeline("exiting...");
}
