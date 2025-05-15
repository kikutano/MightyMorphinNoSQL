#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void print_query_result(DocumentCollection *collection) {
  if (collection == NULL) {
    printf("> No results to display.\n");
    return;
  }

  if (collection->size == 0) {
    printf("> Query returned 0 results.\n");
    return;
  }

  printf("> Query returned %u results:\n", collection->size);

  for (unsigned int i = 0; i < collection->size; ++i) {
    printf("- Id: %u", collection->items[i].id);
    if (collection->with_content && collection->items[i].content != NULL) {
      printf(", Content: %s", collection->items[i].content);
    }
    printf("\n");
  }
}

void print_database_metadata(const char *file_name) {
  FILE *db_file = fopen(file_name, "r+b");

  if (!db_file) {
    printf("Error: cannot open db file.\n");
    return;
  }

  fseek(db_file, 0, SEEK_SET);

  printf("> Database Metadata:\n");
  printf("> Database Tables:\n");
  char buffer[256];
  while (fgets(buffer, sizeof(buffer), db_file)) {
    printf(" - %s", buffer);
  }

  fclose(db_file);

  printf("\n");
  printf("> Database Metadata end.\n");
}

void print_database_table(const char *file_name) {
  FILE *file = fopen(file_name, "r+b");

  if (!file) {
    printf("> Error: cannot open db file.\n");
    return;
  }

  fseek(file, 0, SEEK_SET);

  uint32_t id, content_size;

  printf("> Db Rows:\n");
  while (fread(&id, sizeof(uint32_t), 1, file)) {
    fread(&content_size, sizeof(uint32_t), 1, file);

    char *content = malloc(content_size);
    if (!content) {
      printf("> Error during memory allocation.\n");
      return;
    }

    fread(content, sizeof(char), content_size, file);
    printf("> Id: %u, Content: %s\n", id, content);

    free(content);
  }
}

//---
void print_db_file(FILE *db) {
  if (!db) {
    printf("Error: cannot open db file.\n");
    return;
  }

  fseek(db, 0, SEEK_SET);

  uint32_t id, content_size;

  printf("Db Rows:\n");
  while (fread(&id, sizeof(uint32_t), 1, db)) {
    fread(&content_size, sizeof(uint32_t), 1, db);

    char *content = malloc(content_size);
    if (!content) {
      printf("Error during memory allocation.\n");
      return;
    }

    fread(content, sizeof(char), content_size, db);
    printf("Id: %u, Content: %s\n", id, content);

    free(content);
  }
}

void print_index_file(FILE *db_indexes) {
  if (!db_indexes) {
    printf("Error: cannot open index file.\n");
    return;
  }

  fseek(db_indexes, 0, SEEK_SET);

  uint32_t id;
  uint64_t offset;

  printf("Indexes:\n");
  while (fread(&id, sizeof(uint32_t), 1, db_indexes)) {
    fread(&offset, sizeof(uint64_t), 1, db_indexes);
    printf("Id: %u, Offset: %llu\n", id, offset);
  }
}

/*void create_dummy_rows(int how_many) {
  create_database("bigdatabase");
  Database *db = open_database_connection("bigdatabase");
  create_database_table(db, "bigtable.db");
  Table *table = open_database_table_connection(db, "bigtable");

  for (uint64_t i = 0; i < how_many; i++) {
    char content[256];
    sprintf(content,
            "{\"data1\":1,\"data2\":2,\"datastring\":\"Amazing result %lu\"}",
            (uint64_t)i);
    insert(table, i, content);
  }

  close_database_connection(db);
}*/

/* USAGE
uint32_t rows = 10000000;
  // create_dummy_rows(rows);

  Database *db = open_database_connection("bigdatabase");
  Table *table = open_database_table_connection(db, "bigtable");

  LARGE_INTEGER frequency, start, end;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

  char *content = get_by_id(table, rows - 1);

  QueryPerformanceCounter(&end);

  double elapsed_time_ms =
      (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
  printf("Elapsed time: %.2f ms\n", elapsed_time_ms);

  printf("> Content found, content: %s\n", content);

  free(content);
  close_database_table_connection(table);
  close_database_connection(db);
*/