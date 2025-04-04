#ifndef  UTILS_H_
#define  UTILS_H_

#include <stdio.h>

void print_database_metadata(const char *file_name);
void print_db_file(FILE *db);
void print_index_file(FILE *db_indexes);

#endif