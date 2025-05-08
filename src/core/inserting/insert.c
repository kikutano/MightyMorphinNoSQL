#include "insert.h"
#include <stdint.h>
#include <string.h>

uint64_t append_content(Table *table, uint32_t id, const char *content);
void append_index(Table *table, const uint32_t id, const uint64_t offset);

/*
====================================================================
ROW PROTOTYPE:
[id][content_size][content + '\0']
example:
[10][19][my amazing content]
====================================================================
*/
void insert(Table *table, uint32_t id, const char *content) {
  uint64_t offset = append_content(table, id, content);
  append_index(table, id, offset);
}

uint64_t append_content(Table *table, uint32_t id, const char *content) {
  // Go to the end of the file
  fseek(table->file, 0, SEEK_END);
  // Get the current offset (file size)
  uint64_t offset = ftell(table->file);

  // Write first the Id of the row [id]
  fwrite(&id, sizeof(uint32_t), 1, table->file);

  // Write the content size of the content [content_size]
  uint32_t content_size = strlen(content) + 1;
  fwrite(&content_size, sizeof(uint32_t), 1, table->file);

  // Write the complete content [my amazing content]
  fwrite(content, sizeof(char), content_size, table->file);
  return offset;
}

/*
====================================================================
INDEX PROTOTYPE:
[id][offset]
example:
[10][1955504]

Offset indicates the position of the the content with that id
into the table file.
====================================================================
*/
void append_index(Table *table, const uint32_t id, const uint64_t offset) {
  // Go to the end of offset file
  fseek(table->ids_indexes, 0, SEEK_END);

  // Write the id of the element [id]
  fwrite(&id, sizeof(uint32_t), 1, table->ids_indexes);
  // Write the offset of the element into the table file
  fwrite(&offset, sizeof(uint64_t), 1, table->ids_indexes);

  fflush(table->file);
  fflush(table->ids_indexes);
}