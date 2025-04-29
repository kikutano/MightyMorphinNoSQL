#include "log.h"
#include <stdarg.h>
#include <stdio.h>

void log(const char *const log, ...) {
  va_list args;
  va_start(args, log);

  printf("> log: ");
  vprintf(log, args);

  printf("\n");
  va_end(args);
}