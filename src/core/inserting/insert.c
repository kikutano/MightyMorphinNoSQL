#include "insert.h"
#include <string.h>

void insert(FILE *db_indexes, FILE *db, uint32_t id, const char *content) {
  fseek(db, 0, SEEK_END);      // Go to the end of the file
  uint64_t offset = ftell(db); // Get the current offset (file size)

  /* Write first the Id of the row */
  fwrite(&id, sizeof(uint32_t), 1, db);

  /* Write the content of the row. The content has the null terminator, so
   * strlen() + 1*/
  uint32_t content_size = strlen(content) + 1;
  fwrite(&content_size, sizeof(uint32_t), 1, db); // Write the size
  fwrite(content, sizeof(char), content_size, db);

  /* Write the index with the offset */
  fseek(db_indexes, 0, SEEK_END);
  fwrite(&id, sizeof(uint32_t), 1, db_indexes);
  fwrite(&offset, sizeof(uint64_t), 1, db_indexes);
}