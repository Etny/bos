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

void exception_handler(struct itr_data* reg) {
  char buf[100];
  char* head = strcpy(buf, "received unhandled interupt: ");
  head = itos(reg->int_code, SLICE(head, 10));
  head = strcpy(head, " and error code ");
  head = itohex(reg->err_code, SLICE(head, 10));
  panic_from(buf, reg->eip, reg->ebp);
}
