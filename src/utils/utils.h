#ifndef UTILS_H_
#define UTILS_H_

#include "../core/database/document.h"
#include <stdio.h>

void print_database_metadata(const char *file_name);
void print_database_table(const char *file_name);
void print_db_file(FILE *db);
void print_index_file(FILE *db_indexes);
void create_dummy_rows(int how_many);

void print_query_result(DocumentCollection *collection);

#endif