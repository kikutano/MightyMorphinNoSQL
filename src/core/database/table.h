#ifndef TABLE_H_
#define TABLE_H_

#include <stdio.h>

typedef struct {
    char name[256];
    FILE *file;
    FILE *ids_indexes;
    FILE *metadata;
} Table;

#endif