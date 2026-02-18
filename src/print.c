#include "print.h"

#include "slice.h"
#include "str.h"
#include "terminal.h"

void __print(struct _print_entry entry) {
  struct slice_char buf = SLICE_STACK(char, 100);
  switch (entry.kind) {
    case INT:
      itos(entry.data.data_int, buf);
      term_writeline(buf.ptr);
      break;
    case STR:
      term_writeline(entry.data.data_str);
      break;
    case SLICE:
      for (size_t i = 0; i < entry.data.data_slice.len; i++)
        term_putchar(entry.data.data_slice.ptr[i]);
      term_writeline("");
      break;
  }
}
