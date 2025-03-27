#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void insert(uint32_t id, const char *content) {
  fseek(current_db_file, 0, SEEK_END); // Go to the end of the file
  uint64_t offset =
      ftell(current_db_file); // Get the current offset (file size)

  /* Write first the Id of the row */
  fwrite(&id, sizeof(uint32_t), 1, current_db_file);

  /* Write the content of the row. The content has the null terminator, so
   * strlen() + 1*/
  uint32_t content_size = strlen(content) + 1;
  fwrite(&content_size, sizeof(uint32_t), 1, current_db_file); // Write the size
  fwrite(content, sizeof(char), content_size, current_db_file);

  /* Write the index with the offset */
  fseek(current_db_file_indexes, 0, SEEK_END);
  fwrite(&id, sizeof(uint32_t), 1, current_db_file_indexes);
  fwrite(&offset, sizeof(uint64_t), 1, current_db_file_indexes);
}

#define INDEX_ENTRY_SIZE                                                       \
  (sizeof(uint32_t) + sizeof(uint64_t)) // 4 + 8 = 12 bytes

uint64_t find_offset_by_id(uint32_t target_id) {
  uint32_t id;
  uint64_t offset;

  long left = 0, right, mid;

  /* TODO: Use an header metadata file for number of indexes! */
  fseek(current_db_file_indexes, 0, SEEK_END);
  right = ftell(current_db_file_indexes) / INDEX_ENTRY_SIZE - 1;

  while (left <= right) {
    mid = left + (right - left) / 2;

    // set position on the mid index
    fseek(current_db_file_indexes, mid * INDEX_ENTRY_SIZE, SEEK_SET);
    fread(&id, sizeof(uint32_t), 1, current_db_file_indexes);

    if (id == target_id) {
      fread(&offset, sizeof(uint64_t), 1, current_db_file_indexes);

      // TODO: Pass a pointer? Or a reference?
      return offset;
    }

    if (id < target_id) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return (uint64_t)-1;
}

char *get_by_id(uint32_t target_id) {
  uint64_t offset = find_offset_by_id(target_id);

  if (offset == -1) {
    printf("Id %u not found.\n", target_id);
    return NULL;
  }

  /* Go to offset position on db */
  fseek(current_db_file, offset, SEEK_SET);

  // Read the id and the size of the content
  uint32_t stored_id, content_size;
  fread(&stored_id, sizeof(uint32_t), 1, current_db_file);
  fread(&content_size, sizeof(uint32_t), 1, current_db_file);

  char *buffer = (char *)malloc(content_size);

  /* Put inside buffer the content of the row */
  fread(buffer, sizeof(char), content_size, current_db_file);
  buffer[content_size - 1] = '\0'; 

  return buffer;
}

//-- Utils
void print_db_file() {
  if (!current_db_file) {
    printf("Error: cannot open db file.\n");
    return;
  }

  fseek(current_db_file, 0, SEEK_SET);

  uint32_t id, content_size;

  printf("Db Rows:\n");
  while (fread(&id, sizeof(uint32_t), 1, current_db_file)) {
    fread(&content_size, sizeof(uint32_t), 1, current_db_file);

    char *content = malloc(content_size);
    if (!content) {
      printf("Error during memory allocation.\n");
      return;
    }

    fread(content, sizeof(char), content_size, current_db_file);
    printf("Id: %u, Content: %s\n", id, content);

    free(content);
  }
}

void print_index_file() {
  if (!current_db_file_indexes) {
    printf("Error: cannot open index file.\n");
    return;
  }

  fseek(current_db_file_indexes, 0, SEEK_SET);

  uint32_t id;
  uint64_t offset;

  printf("Indexes:\n");
  while (fread(&id, sizeof(uint32_t), 1, current_db_file_indexes)) {
    fread(&offset, sizeof(uint64_t), 1, current_db_file_indexes);
    printf("Id: %u, Offset: %lu\n", id, offset);
  }
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

int main(int argc, char *argv[]) {
  open_db_file("mydb");

  /*insert(current_id, "cicciopasticcio1");
  ++current_id;

  insert(current_id, "cicciopasticcio2");
  ++current_id;

  insert(current_id, "cicciopasticcio3");
  ++current_id;

  insert(current_id, "cicciopasticcio4");
  ++current_id;

  insert(current_id, "cicciopasticcio5");
  ++current_id;*/

  char *data = get_by_id(2); 
  if (data) {
    printf("Data found: %s\n", data); 
  } else {
    printf("Data not found.\n");
  }

  printf("\n\n");
  print_db_file();
  printf("\n\n");
  print_index_file();

  close_db_file();

  return 0;
}