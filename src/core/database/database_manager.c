#include "database_manager.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_DB_SUFFIX "%s.db"
#define FILE_DB_TABLE_SUFFIX "%s_%s_table.db"
#define FILE_DB_TABLE_METADATA_SUFFIX "%s_%s_table_metadata.db"
#define FILE_DB_TABLE_INDEXES_SUFFIX "%s_%s_table_ids_indexes.db"

/* TODO:
    - Incapsulate naming costruction in a function
    - Incapsulate opening file inside a single function for table, indexes and
   metadata.
 */

void create_database(const char *name) {
  char db_file_name[256];
  sprintf(db_file_name, FILE_DB_SUFFIX, name);

  FILE *db_file = fopen(db_file_name, "a+b");
  if (!db_file) {
    db_file = fopen(db_file_name, "w+b");
  }
}

Database *open_database_connection(const char *name) {
  char db_file_name[256];
  sprintf(db_file_name, FILE_DB_SUFFIX, name);

  FILE *db_file = fopen(db_file_name, "a+b");
  if (!db_file) {
    printf("Error opening database file: %s\n", db_file_name);
    return NULL;
  }

  Database *database = malloc(sizeof(Database));
  database->metadata = db_file;

  /* TODO database->tables = malloc(sizeof(Table) * 10); Allocate space for 10
   * tables (example) */

  strncpy(database->name, name, sizeof(database->name) - 1);
  database->name[sizeof(database->name) - 1] = '\0';

  return database;
}

void close_database_connection(Database *database) {
  fclose(database->metadata);
  free(database);
}

void create_database_table(Database *database, const char *name) {
  /* Skip if table exists*/
  char db_file_name[256];
  sprintf(db_file_name, FILE_DB_SUFFIX, database->name);
  FILE *db_file = fopen(db_file_name, "a+b");

  fseek(db_file, 0, SEEK_SET);
  char buffer[256];
  while (fgets(buffer, sizeof(buffer), db_file)) {
    if (strstr(buffer, name) != NULL) {
      printf("> Warning: Table [%s] already exists in database [[%s]]. This "
             "command will be skipped.\n",
             name, database->name);
      fclose(db_file);
      return;
    }
  }

  /* Create table file */
  char table_file_name[256];
  sprintf(table_file_name, FILE_DB_TABLE_SUFFIX, database->name, name);

  FILE *table_file = fopen(table_file_name, "a+b");
  if (!table_file) {
    table_file = fopen(table_file_name, "w+b");
  }

  /* Create table metadata file */
  char table_metadata_file_name[256];
  sprintf(table_metadata_file_name, FILE_DB_TABLE_METADATA_SUFFIX,
          database->name, name);

  FILE *table_metadata_file = fopen(table_metadata_file_name, "a+b");
  if (!table_metadata_file) {
    table_metadata_file = fopen(table_metadata_file_name, "w+b");
  }

  /* Create table indexes file */
  char db_file_name_indexes[256];
  sprintf(db_file_name_indexes, FILE_DB_TABLE_INDEXES_SUFFIX, database->name,
          name);

  FILE *db_file_indexes = fopen(db_file_name_indexes, "a+b");

  if (!db_file_indexes) {
    db_file_indexes = fopen(db_file_name_indexes, "w+b");
  }

  /* Append table on db metadata file */
  char table_name_on_file[256];
  sprintf(table_name_on_file, "%s\n", name);
  fwrite(table_name_on_file, sizeof(char), strlen(table_name_on_file), db_file);

  printf("> Table [%s] created in database [[%s]].\n", name, database->name);

  fclose(db_file);
}

Table *open_database_table_connection(Database *database, const char *name) {
  char table_file_name[256];
  sprintf(table_file_name, FILE_DB_TABLE_SUFFIX, database->name, name);

  FILE *table_file = fopen(table_file_name, "a+b");
  if (!table_file) {
    printf("> Error opening table file: %s\n", table_file_name);
    return NULL;
  }

  char db_file_name_indexes[256];
  sprintf(db_file_name_indexes, FILE_DB_TABLE_INDEXES_SUFFIX, database->name,
          name);
  FILE *db_file_indexes = fopen(db_file_name_indexes, "a+b");

  if (!db_file_indexes) {
    printf("> Error opening table file indexes: %s\n", db_file_name_indexes);
    return NULL;
  }

  char table_metadata_file_name[256];
  sprintf(table_metadata_file_name, FILE_DB_TABLE_METADATA_SUFFIX,
          database->name, name);

  FILE *table_metadata_file = fopen(table_metadata_file_name, "a+b");
  if (!table_metadata_file) {
    printf("> Error opening table file metadata: %s\n",
           table_metadata_file_name);
    return NULL;
  }

  Table *table = malloc(sizeof(Table));
  table->file = table_file;
  table->ids_indexes = db_file_indexes;
  table->metadata = table_metadata_file;

  return table;
}

void close_database_table_connection(Table *table) {
  fclose(table->metadata);
  fclose(table->file);
  fclose(table->ids_indexes);
  free(table);
}
