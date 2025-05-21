#include "database_manager.h"
#include "../../logs/mm_log.h"
#include "mmdb_file_manager.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_database(const char *name) {
  FILE *db_file = mmdb_get_database_file(name);
  if (db_file) {
    mm_log("Warning: database '%s' command will be skipped!", name);
    fclose(db_file);
    return;
  }

  mmdb_create_database_file(name);
}

void delete_database(const char *db_name) {
  FILE *db_file = mmdb_get_database_file(db_name);

  // remove tables files
  char table_name[256];
  while (fgets(table_name, sizeof(table_name), db_file)) {
    size_t len = strlen(table_name);
    if (table_name[len - 1] == '\n') {
      table_name[len - 1] = '\0';
    }

    mmdb_delete_database_table_file(db_name, table_name);
    mmdb_delete_database_table_metadata_file(db_name, table_name);
    mmdb_delete_database_table_ids_indexes_metadata_file(db_name, table_name);
  }

  fclose(db_file);
  mmdb_delete_database_file(db_name);
}

Database *open_database_connection(const char *name) {
  FILE *db_file = mmdb_get_database_file(name);

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
  FILE *db_file = mmdb_get_database_file(database->name);

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

  mmdb_create_database_table_file(database->name, name);
  mmdb_create_database_table_metadata_file(database->name, name);
  mmdb_create_database_table_ids_indexes_file(database->name, name);

  /* Append table on db metadata file */
  char table_name_on_file[256];
  sprintf(table_name_on_file, "%s\n", name);
  fwrite(table_name_on_file, sizeof(char), strlen(table_name_on_file), db_file);

  mm_log("Table %s created in database %s", name, database->name);
  fclose(db_file);
}

Table *open_database_table_connection(Database *database, const char *name) {
  if (database == NULL) {
    printf("> Error. Database is null. Did you opened a connection?");
    return NULL;
  }

  Table *table = malloc(sizeof(Table));
  table->file = mmdb_get_database_table_file(database->name, name);
  table->metadata = mmdb_get_database_table_metadata_file(database->name, name);
  table->ids_indexes =
      mmdb_get_database_table_ids_indexes_file(database->name, name);

  return table;
}

void close_database_table_connection(Table *table) {
  fclose(table->metadata);
  fclose(table->file);
  fclose(table->ids_indexes);
  free(table);
}
