#ifndef __CPU_H
#define __CPU_H

struct vendor_id {
  char name[13];
};

void read_vendor_id(char *buf);
struct vendor_id get_vendor_id(void);

#endif  // !__CPU_H
