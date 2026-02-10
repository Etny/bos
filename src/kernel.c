#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ia32/gdt.h"
#include "terminal.h"

void kernel_main(void) {
  setup_gdt();

  term_init();

  term_writeline("Hello there user!");
  term_writeline("I can write stuff here!!!");
}
