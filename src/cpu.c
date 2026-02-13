#include "cpu.h"

struct vendor_id get_vendor_id(void) {
  struct vendor_id id;
  read_vendor_id(id.name);

  return id;
}
