#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
    printf("Id: %u, Offset: %lu\n", id, offset);
  }
}