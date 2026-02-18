#ifndef __ERROR_H
#define __ERROR_H

#include <stdint.h>

#include "idt.h"

// The default exception handler
void exception_handler(struct itr_data *reg);

#endif
