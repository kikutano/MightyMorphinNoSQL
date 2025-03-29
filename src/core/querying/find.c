#include "find.h"
#include <stdlib.h>

#define INDEX_ENTRY_SIZE                                                       \
  (sizeof(uint32_t) + sizeof(uint64_t)) // 4 + 8 = 12 bytes

uint64_t find_offset_by_id(FILE *db_indexes, uint32_t target_id) {
  uint32_t id;
  uint64_t offset;

  long left = 0, right, mid;

  /* TODO: Use an header metadata file for number of indexes! */
  fseek(db_indexes, 0, SEEK_END);
  right = ftell(db_indexes) / INDEX_ENTRY_SIZE - 1;

  while (left <= right) {
    mid = left + (right - left) / 2;

    // set position on the mid index
    fseek(db_indexes, mid * INDEX_ENTRY_SIZE, SEEK_SET);
    fread(&id, sizeof(uint32_t), 1, db_indexes);

    if (id == target_id) {
      fread(&offset, sizeof(uint64_t), 1, db_indexes);

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

char *get_by_id(FILE *db_indexes, FILE *db, uint32_t target_id) {
  uint64_t offset = find_offset_by_id(db_indexes, target_id);

  if (offset == -1) {
    printf("Id %u not found.\n", target_id);
    return NULL;
  }

  /* Go to offset position on db */
  fseek(db, offset, SEEK_SET);

  // Read the id and the size of the content
  uint32_t stored_id, content_size;
  fread(&stored_id, sizeof(uint32_t), 1, db);
  fread(&content_size, sizeof(uint32_t), 1, db);

  char *buffer = (char *)malloc(content_size);

  /* Put inside buffer the content of the row */
  fread(buffer, sizeof(char), content_size, db);
  buffer[content_size - 1] = '\0';

  return buffer;
}