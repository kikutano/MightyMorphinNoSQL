#ifndef DATABASE_H_
#define DATABASE_H_

#include <stdio.h>
#include "table.h"

typedef  struct {
    char name[256];
    FILE *metadata;
} Database;

#endif