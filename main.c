#include "src/core/inserting/insert.h"
#include "src/core/querying/find.h"
#include "src/utils/utils.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

FILE *current_db_file;
FILE *current_db_file_header;
FILE *current_db_file_indexes;
char db_file_name[256];
char db_file_name_header[256];
char db_file_name_indexes[256];
uint32_t current_id = 1;

#define FILE_SUFFIX "%s.db"
#define FILE_HEADER_SUFFIX "%s_header.db"
#define FILE_INDEXES_SUFFIX "%s_ids_indexes.db"
#define HEADER_INDEX_FORMAT "index_counter:%lu"

void open_db_file(char *name) {
  // database
  sprintf(db_file_name, FILE_SUFFIX, name);
  current_db_file = fopen(db_file_name, "a+b");

  // create if not exists
  if (!current_db_file) {
    current_db_file = fopen(db_file_name, "w+b");
  }

  // indexes
  sprintf(db_file_name_indexes, FILE_INDEXES_SUFFIX, name);
  current_db_file_indexes = fopen(db_file_name_indexes, "r+b");

  // create if not exists
  if (!current_db_file_indexes) {
    current_db_file_indexes = fopen(db_file_name_indexes, "w+b");
  }
}

void close_db_file() {
  fclose(current_db_file);
  fclose(current_db_file_header);
  fclose(current_db_file_indexes);
}

/*
        WORKING ON:
        4. Can we remove \n from file?
        2. Indexing searching (search by a param)

        TODO:
        6. memcpy VS fprintf
        7. Files are not db, but entities!
        8. CRUD operations
        9. Concurrency anyone?
        10. Console table visualizator
        11. Use multiple files for indexes and data
        12. Use a metadata file

        INDEXING:
        1. Use a b-tree to store and search indexes?
        3. Load indexes in memory RAM

        QUERY LANGUAGE:
        1. Do we need a query language?
*/

int main() {
  open_db_file("mydb");

  /*insert(current_db_file_indexes, current_db_file, current_id,
         "cicciopasticcio1");
  ++current_id;

  insert(current_db_file_indexes, current_db_file, current_id,
         "cicciopasticcio2");
  ++current_id;

  insert(current_db_file_indexes, current_db_file, current_id,
         "cicciopasticcio3");
  ++current_id;

  insert(current_db_file_indexes, current_db_file, current_id,
         "cicciopasticcio4");
  ++current_id;

  insert(current_db_file_indexes, current_db_file, current_id,
         "cicciopasticcio5");
  ++current_id;*/

  char *data = get_by_id(current_db_file_indexes, current_db_file, 4);
  if (data) {
    printf("Data found: %s\n", data);
  } else {
    printf("Data not found.\n");
  }

  printf("\n\n");
  print_db_file(current_db_file);
  printf("\n\n");
  print_index_file(current_db_file_indexes);

  close_db_file();

  return 0;
}