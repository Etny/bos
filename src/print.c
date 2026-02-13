#include "print.h"

#include "slice.h"
#include "str.h"
#include "terminal.h"

void __print(struct _print_entry entry) {
  char buf[100];
  switch (entry.kind) {
    case INT:
      itos(entry.data.data_int, slice(buf));
      term_writeline(buf);
      break;
    case STR:
      term_writeline(entry.data.data_str);
      break;
  }
}
