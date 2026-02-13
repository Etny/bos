#include "except.h"

#include "panic.h"
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
  head = itos(reg->int_code, slice_len(head, 10));
  head = strcpy(head, " and error code ");
  // head = itos(reg->err_code, head, 10);
  panic(buf);

  // if (reg->int_code == 8) *((char*)0xB8000) = '0' + reg->int_code;
  // *((char*)0xB8001) = 0x0F;  // White on Black
}
