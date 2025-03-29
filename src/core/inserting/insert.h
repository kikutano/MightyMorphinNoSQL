#ifndef INSERT_H_
#define INSERT_H_

#include <stdint.h>
#include <stdio.h>

void insert(FILE *db_indexes, FILE *db, uint32_t id, const char *content);

#endif