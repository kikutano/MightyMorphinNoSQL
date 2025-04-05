#include "insert.h"
#include <string.h>

void insert(Table *table, uint32_t id, const char *content) {
  /* Row prototype:
    [id][content_size][content + '\0']
   */
  fseek(table->file, 0, SEEK_END);      // Go to the end of the file
  uint64_t offset = ftell(table->file); // Get the current offset (file size)

  /* Write first the Id of the row */
  fwrite(&id, sizeof(uint32_t), 1, table->file);

  /* Write the content of the row. The content has the null terminator, so
   * strlen() + 1*/
  uint32_t content_size = strlen(content) + 1;
  fwrite(&content_size, sizeof(uint32_t), 1, table->file); // Write the size
  fwrite(content, sizeof(char), content_size, table->file);

  /* Write the index with the offset */
  fseek(table->ids_indexes, 0, SEEK_END);
  fwrite(&id, sizeof(uint32_t), 1, table->ids_indexes);
  fwrite(&offset, sizeof(uint64_t), 1, table->ids_indexes);

  fflush(table->file);
  fflush(table->ids_indexes);
}