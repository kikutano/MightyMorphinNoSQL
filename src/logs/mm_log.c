#include "mm_log.h"
#include <stdarg.h>
#include <stdio.h>

void mm_log(const char *const log, ...) {
  va_list args;
  va_start(args, log);

  printf("> log: ");
  vprintf(log, args);

  printf("\n");
  va_end(args);
}