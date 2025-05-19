#include "database_manager.h"
#include "../../logs/mm_log.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *FILE_DB_SUFFIX = ".db";
static const char *FILE_DB_TABLE_SUFFIX = "_table.db";
static const char *FILE_DB_TABLE_METADATA_SUFFIX = "_table_metadata.db";
static const char *FILE_DB_TABLE_INDEXES_SUFFIX = "_table_ids_indexes.db";

void create_database(const char *name) {
  if (strlen(name) > 252) {
    mm_log("Error: name too long to create a valid DB file name.\n");
    return;
  }

  char db_file_name[256];
  snprintf(db_file_name, sizeof(db_file_name), "%s%s", name, FILE_DB_SUFFIX);

  FILE *existing_file = fopen(db_file_name, "r");
  if (existing_file) {
    fclose(existing_file);
    mm_log("Warning: database '%s' command will be skipped!\n", db_file_name);
    return;
  }

  FILE *db_file = fopen(db_file_name, "w+b");
  fclose(db_file);
  mm_log("Database %s created! \n", name);
}

void delete_database(const char *db_name) {
  char db_file_name[256];
  snprintf(db_file_name, sizeof(db_file_name), "%s%s", db_name, FILE_DB_SUFFIX);
  FILE *db_file = fopen(db_file_name, "r+b");
  fseek(db_file, 0, SEEK_SET);

  // remove tables files
  char table_name[256];
  while (fgets(table_name, sizeof(table_name), db_file)) {
    size_t len = strlen(table_name);
    if (table_name[len - 1] == '\n') {
      table_name[len - 1] = '\0';
    }

    //remove table file
    char table_file_name[256];
    snprintf(table_file_name, sizeof(table_file_name), "%s_%s%s", db_name,
             table_name, FILE_DB_TABLE_SUFFIX);

    if (remove(table_file_name) == 0) {
      mm_log("File %s removed!", table_file_name);
    } else {
      perror("Error during table file elimination");
      printf("Code error: %d on table %s", errno, table_file_name);
    }

    // remove table metadata
    char table_metadata_file_name[256];
    snprintf(table_metadata_file_name, sizeof(table_metadata_file_name),
             "%s_%s%s", db_name, table_name, FILE_DB_TABLE_METADATA_SUFFIX);

    if (remove(table_metadata_file_name) == 0) {
      mm_log("File %s removed!", table_metadata_file_name);
    } else {
      perror("Error during table metadata file elimination");
      printf("Code error: %d on table %s", errno, table_metadata_file_name);
    }

    // remove table ids indexes
    char table_ids_indexes_file_name[256];
    snprintf(table_ids_indexes_file_name, sizeof(table_ids_indexes_file_name),
             "%s_%s%s", db_name, table_name, FILE_DB_TABLE_INDEXES_SUFFIX);

    if (remove(table_ids_indexes_file_name) == 0) {
      mm_log("File %s removed!", table_ids_indexes_file_name);
    } else {
      perror("Error during table indexes file elimination");
      printf("Code error: %d on table %s", errno, table_metadata_file_name);
    }
  }

  // remove database file
  fclose(db_file);
  if (remove(db_file_name) == 0) {
    mm_log("File %s removed!", db_file_name);
  } else {
    perror("Error during file elimination!");
    printf("Code error: %d\n", errno);
  }
}

Database *open_database_connection(const char *name) {
  char db_file_name[256];
  snprintf(db_file_name, sizeof(db_file_name), "%s%s", name, FILE_DB_SUFFIX);

  FILE *db_file = fopen(db_file_name, "a+b");
  if (!db_file) {
    printf("Error opening database file: %s\n", db_file_name);
    return NULL;
  }

  Database *database = malloc(sizeof(Database));
  database->metadata = db_file;

  strncpy(database->name, name, sizeof(database->name) - 1);
  database->name[sizeof(database->name) - 1] = '\0';

  mm_log("Connection with database %s established! Enjoy!", name);
  return database;
}

void close_database_connection(Database *database) {
  fclose(database->metadata);
  free(database);
}

void create_database_table(Database *database, const char *name) {
  /* Skip if table exists*/
  char db_file_name[256];
  snprintf(db_file_name, sizeof(db_file_name), "%s%s", database->name,
           FILE_DB_SUFFIX);
  FILE *db_file = fopen(db_file_name, "a+b");

  fseek(db_file, 0, SEEK_SET);
  char buffer[256];
  while (fgets(buffer, sizeof(buffer), db_file)) {
    if (strstr(buffer, name) != NULL) {
      mm_log("Warning: Table %s already exists in database %s. This command "
             "will be skipped.",
             name, database->name);
      fclose(db_file);
      return;
    }
  }

  /* Create table file */
  char table_file_name[256];
  snprintf(table_file_name, sizeof(table_file_name), "%s_%s%s", database->name,
           name, FILE_DB_TABLE_SUFFIX);

  FILE *table_file = fopen(table_file_name, "a+b");
  if (!table_file) {
    table_file = fopen(table_file_name, "w+b");
  }

  /* Create table metadata file */
  char table_metadata_file_name[256];
  snprintf(table_metadata_file_name, sizeof(table_metadata_file_name),
           "%s_%s%s", database->name, name, FILE_DB_TABLE_METADATA_SUFFIX);

  FILE *table_metadata_file = fopen(table_metadata_file_name, "a+b");
  if (!table_metadata_file) {
    table_metadata_file = fopen(table_metadata_file_name, "w+b");
  }

  /* Create table indexes file */
  char db_file_name_indexes[256];
  snprintf(db_file_name_indexes, sizeof(db_file_name_indexes), "%s_%s%s",
           database->name, name, FILE_DB_TABLE_INDEXES_SUFFIX);

  FILE *db_file_indexes = fopen(db_file_name_indexes, "a+b");

  if (!db_file_indexes) {
    db_file_indexes = fopen(db_file_name_indexes, "w+b");
  }

  /* Append table on db metadata file */
  char table_name_on_file[256];
  sprintf(table_name_on_file, "%s\n", name);
  fwrite(table_name_on_file, sizeof(char), strlen(table_name_on_file), db_file);

  mm_log("Table %s created in database %s", name, database->name);
  fclose(db_file);
  fclose(table_file);
  fclose(table_metadata_file);
  fclose(db_file_indexes);
}

Table *open_database_table_connection(Database *database, const char *name) {
  if (database == NULL) {
    printf("> Error. Database is null. Did you opened a connection?");
    return NULL;
  }

  char table_file_name[256];
  snprintf(table_file_name, sizeof(table_file_name), "%s%s%s",
           FILE_DB_TABLE_SUFFIX, database->name, name);

  FILE *table_file = fopen(table_file_name, "a+b");
  if (!table_file) {
    printf("> Error opening table file: %s\n", table_file_name);
    return NULL;
  }

  char db_file_name_indexes[256];
  snprintf(db_file_name_indexes, sizeof(db_file_name_indexes), "%s%s%s",
           FILE_DB_TABLE_INDEXES_SUFFIX, database->name, name);
  FILE *db_file_indexes = fopen(db_file_name_indexes, "a+b");

  if (!db_file_indexes) {
    printf("> Error opening table file indexes: %s\n", db_file_name_indexes);
    return NULL;
  }

  char table_metadata_file_name[256];
  snprintf(table_metadata_file_name, sizeof(table_metadata_file_name), "%s%s%s",
           FILE_DB_TABLE_METADATA_SUFFIX, database->name, name);

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
