#include "panic.h"

#include "terminal.h"

void panic(const char *error) {
  term_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
  term_writeline("---PANIC---");
  term_writeline(error);

  for (;;);
}
