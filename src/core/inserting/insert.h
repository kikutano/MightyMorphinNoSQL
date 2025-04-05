#ifndef INSERT_H_
#define INSERT_H_

#include <stdint.h>
#include <stdio.h>
#include "../database/table.h"

void insert(Table *table, uint32_t id, const char *content);

#endif