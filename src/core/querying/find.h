#ifndef FIND_H_
#define FIND_H_

#include <stdint.h>
#include <stdio.h>

uint64_t find_offset_by_id(FILE *current_db_file_indexes, uint32_t target_id);
char *get_by_id(FILE *db_indexes, FILE *db, uint32_t target_id);

#endif