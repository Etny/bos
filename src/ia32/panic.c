#include "panic.h"

#include "slice.h"
#include "terminal.h"

void _panic(const char *error, const char *file, const char *line) {
  asm volatile("cli");
  term_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
  term_writeline("--!PANIC!--");
  term_write("At ");
  term_write(file);
  term_write(":");
  term_writeline(line);
  term_writeline(error);

  for (;;);
}
