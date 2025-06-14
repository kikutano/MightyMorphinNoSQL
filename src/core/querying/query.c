#include "query.h"
#include "../../logs/mm_log.h"
#include "../database/database_manager.h"
#include <stdio.h>
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

/*
Todo:
- add content to items
- add where support to columns
 */
DocumentCollection *perform_select(Database *database, Command *query) {
  Table *table = open_database_table_connection(database, query->params[0]);

  DocumentCollection *collection = malloc(sizeof(DocumentCollection));
  collection->items = NULL;
  collection->size = 0;
  collection->with_content = 1;

  uint32_t where_id = 0;
  uint64_t offset = 0;
  if (query->params_count >= 2) {
    /* If the where clause is indicated, the id value is into params[2]
     * position. We support only where on id. */
    where_id = atoi(query->params[2]);
    offset = find_offset_by_id(table, where_id);

    if (offset == (uint64_t)-1) {
      mm_log("Id %u not found.\n", where_id);

      close_database_table_connection(table);
      return collection;
    }
  }

  // Go on file position where the data start
  // Data is store in this format: [id][content_size][content + '\0']
  fseek(table->file, offset, SEEK_SET);
  do {
    uint32_t stored_id, content_size;
    // Read the [id] as the first data
    size_t read_id = fread(&stored_id, sizeof(uint32_t), 1, table->file);
    // Read the size of the content [content_size]. How chars has the content?
    size_t read_size = fread(&content_size, sizeof(uint32_t), 1, table->file);

    if (read_id != 1 || read_size != 1) {
      break;
    }

    Document *items =
        realloc(collection->items, sizeof(Document) * (collection->size + 1));

    collection->items = items;
    collection->items[collection->size].id = stored_id;

    char *content = malloc(content_size);
    fread(content, sizeof(char), content_size, table->file);
    collection->items[collection->size].content = content;
    collection->size++;

    if (where_id > 0)
      break;
  } while (1);

  close_database_table_connection(table);
  return collection;
}

void free_document_collection(DocumentCollection *collection) {
  if (collection == NULL)
    return;

  for (size_t i = 0; i < collection->size; ++i) {
    if (collection->items[i].content != NULL) {
      free(collection->items[i].content);
    }
  }

  free(collection->items);
  free(collection);
}