#include "except.h"

#include "irq.h"
#include "panic.h"
#include "print.h"
#include "slice.h"
#include "str.h"
#include "terminal.h"

#define test(X) _Generic((X), \
    char* : "asdjad", \
    default : "NO" \
)

void exception_handler(struct registers* reg) {
  char buf[100];
  char* head = strcpy(buf, "Received interupt: ");
  head = itos(reg->int_code, SLICE(head, 10));
  head = strcpy(head, " and error code ");
  head = itohex(reg->err_code, SLICE(head, 10));
  print(buf);
  if (reg->int_code > 32)
    eoi();
  else
    // panic_from(buf, reg->eip, reg->ebp);
    panic(buf);
}
