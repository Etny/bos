#include "asm.h"
#include "gdt.h"
#include "terminal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void kernel_main(void) {
  setup_gdt();

  term_init();

  term_writeline("Hello there user!");
  term_writeline("I can write stuff here!!!");
}
