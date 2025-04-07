#include "find.h"
#include <stdlib.h>

#define INDEX_ENTRY_SIZE                                                       \
  (sizeof(uint32_t) + sizeof(uint64_t)) // 4 + 8 = 12 bytes

uint64_t find_offset_by_id(Table *table, uint32_t target_id) {
  uint32_t id;
  uint64_t offset;

  long left = 0, right, mid;

  /* TODO: Use an header metadata file for number of indexes! */
  fseek(table->ids_indexes, 0, SEEK_END);
  right = ftell(table->ids_indexes) / INDEX_ENTRY_SIZE - 1;

  while (left <= right) {
    mid = left + (right - left) / 2;

    // set position on the mid index
    fseek(table->ids_indexes, mid * INDEX_ENTRY_SIZE, SEEK_SET);
    fread(&id, sizeof(uint32_t), 1, table->ids_indexes);

    if (id == target_id) {
      fread(&offset, sizeof(uint64_t), 1, table->ids_indexes);

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

char *get_by_id(Table *table, uint32_t target_id) {
  uint64_t offset = find_offset_by_id(table, target_id);

  if (offset == (uint64_t)-1) {
    printf("Id %u not found.\n", target_id);
    return NULL;
  }

  /* Go to offset position on db */
  fseek(table->file, offset, SEEK_SET);

  // Read the id and the size of the content
  uint32_t stored_id, content_size;
  fread(&stored_id, sizeof(uint32_t), 1, table->file);
  fread(&content_size, sizeof(uint32_t), 1, table->file);

  char *buffer = (char *)malloc(content_size);

  /* Put inside buffer the content of the row */
  fread(buffer, sizeof(char), content_size, table->file);
  buffer[content_size - 1] = '\0';

  return buffer;
}