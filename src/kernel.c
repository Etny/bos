#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "alloc.h"
#include "asm.h"
#include "gdt.h"
#include "str.h"
#include "terminal.h"

void kernel_main(void) {
  term_init();
  term_writeline("Bad Operating System version " __VERSION ", booting...");

  bool is_protected = is_protected_mode();

  if (is_protected)
    term_writeline("We are running in 32-bit protected mode!");
  else
    term_writeline("Uh-oh, we are running in 16-bit real mode...");

  setup_gdt();

  term_writeline("Hello there user!");
  term_writeline("I can write stuff here!!!");
}
