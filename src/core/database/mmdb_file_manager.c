#include "mmdb_file_manager.h"
#include "../../logs/mm_log.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *FILE_DB_SUFFIX = ".db";
static const char *FILE_DB_TABLE_SUFFIX = "table.db";
static const char *FILE_DB_TABLE_METADATA_SUFFIX = "tablemetadata.db";
static const char *FILE_DB_TABLE_INDEXES_SUFFIX = "tableidsindexes.db";

// create file name
static char *create_file_name(const char *format, ...) {
  va_list args;
  char *result_buffer = NULL;
  int needed_length;

  va_start(args, format);
  needed_length = vsnprintf(NULL, 0, format, args);
  va_end(args);

  if (needed_length < 0) {
    mm_log("Error: Failed to calculate needed length for file path (vsnprintf "
           "error).\n");
    return NULL;
  }

  size_t buffer_size = (size_t)needed_length + 1;

  result_buffer = (char *)malloc(buffer_size);
  if (result_buffer == NULL) {
    perror("malloc failed in mmdb_internal_create_file_path");
    mm_log("Error: Memory allocation failed for file path.\n");
    return NULL;
  }

  va_start(args, format);
  int ret = vsnprintf(result_buffer, buffer_size, format, args);
  va_end(args);

  if (ret < 0 || (size_t)ret >= buffer_size) {
    mm_log(
        "Error: Failed to format file path (vsnprintf error or truncation).\n");
    free(result_buffer);
    return NULL;
  }

  return result_buffer;
}

const char *mmdb_create_database_file_name(const char *db_name) {
  return create_file_name("%s%s", db_name, FILE_DB_SUFFIX);
}

const char *mmdb_create_database_table_file_name(const char *db_name,
                                                 const char *table_name) {
  return create_file_name("%s%s%s", db_name, table_name, FILE_DB_TABLE_SUFFIX);
}

const char *
mmdb_create_database_table_metadata_file_name(const char *db_name,
                                              const char *table_name) {
  return create_file_name("%s%s%s", db_name, table_name,
                          FILE_DB_TABLE_METADATA_SUFFIX);
}

const char *
mmdb_create_database_table_ids_indexes_file_name(const char *db_name,
                                                 const char *table_name) {
  return create_file_name("%s%s%s", db_name, table_name,
                          FILE_DB_TABLE_INDEXES_SUFFIX);
}

// gets
FILE *mmdb_get_file(const char *file_name) {
  FILE *existing_file = fopen(file_name, "r+b");
  
  if (existing_file) {
    fseek(existing_file, 0, SEEK_SET);
    mmdb_free_file_name(file_name);
    return existing_file;
  }

  mmdb_free_file_name(file_name);
  return NULL;
}

FILE *mmdb_get_database_file(const char *db_name) {
  const char *db_file_name = mmdb_create_database_file_name(db_name);
  return mmdb_get_file(db_file_name);
}

FILE *mmdb_get_database_table_file(const char *db_name,
                                   const char *table_name) {
  const char *db_table_file_name =
      mmdb_create_database_table_file_name(db_name, table_name);
  return mmdb_get_file(db_table_file_name);
}

FILE *mmdb_get_database_table_metadata_file(const char *db_name,
                                            const char *table_name) {
  const char *table_metadata_file_name =
      mmdb_create_database_table_metadata_file_name(db_name, table_name);
  return mmdb_get_file(table_metadata_file_name);
}

FILE *mmdb_get_database_table_ids_indexes_file(const char *db_name,
                                               const char *table_name) {
  const char *db_file_name_indexes =
      mmdb_create_database_table_ids_indexes_file_name(db_name, table_name);
  return mmdb_get_file(db_file_name_indexes);
}

// creates
void mmdb_create_file(const char *file_name) {
  FILE *file = fopen(file_name, "w+b");
  fclose(file);
  mm_log("File %s created!", file_name);
}

void mmdb_create_database_file(const char *db_name) {
  const char *db_file_name = mmdb_create_database_file_name(db_name);
  mmdb_create_file(db_file_name);
  mmdb_free_file_name(db_file_name);
}

void mmdb_create_database_table_file(const char *db_name,
                                     const char *table_name) {
  const char *db_table_name =
      mmdb_create_database_table_file_name(db_name, table_name);
  mmdb_create_file(db_table_name);
  mmdb_free_file_name(db_table_name);
}

void mmdb_create_database_table_metadata_file(const char *db_name,
                                              const char *table_name) {
  const char *db_table_metadata_name =
      mmdb_create_database_table_metadata_file_name(db_name, table_name);
  mmdb_create_file(db_table_metadata_name);
  mmdb_free_file_name(db_table_metadata_name);
}

void mmdb_create_database_table_ids_indexes_file(const char *db_name,
                                                 const char *table_name) {
  const char *db_file_name_indexes =
      mmdb_create_database_table_ids_indexes_file_name(db_name, table_name);
  mmdb_create_file(db_file_name_indexes);
  mmdb_free_file_name(db_file_name_indexes);
}

// deletes
void mmdb_delete_file(const char *file_name) {
  if (remove(file_name) == 0) {
    mm_log("File %s removed!", file_name);
  } else {
    perror("Error during table file elimination");
    printf("Code error: %d on table %s", errno, file_name);
  }
}

void mmdb_delete_database_file(const char *db_name) {
  const char *db_file_name = mmdb_create_database_file_name(db_name);
  mmdb_delete_file(db_file_name);
  mmdb_free_file_name(db_file_name);
}

void mmdb_delete_database_table_file(const char *db_name,
                                     const char *table_name) {
  const char *table_file_name =
      mmdb_create_database_table_file_name(db_name, table_name);
  mmdb_delete_file(table_file_name);
  mmdb_free_file_name(table_file_name);
}

void mmdb_delete_database_table_metadata_file(const char *db_name,
                                              const char *table_name) {
  const char *table_metadata_file_name =
      mmdb_create_database_table_metadata_file_name(db_name, table_name);
  mmdb_delete_file(table_metadata_file_name);
  mmdb_free_file_name(table_metadata_file_name);
}

void mmdb_delete_database_table_ids_indexes_metadata_file(
    const char *db_name, const char *table_name) {
  const char *table_ids_indexes_file_name =
      mmdb_create_database_table_ids_indexes_file_name(db_name, table_name);
  mmdb_delete_file(table_ids_indexes_file_name);
  mmdb_free_file_name(table_ids_indexes_file_name);
}

void mmdb_free_file_name(const char *file_name) { free((void *)file_name); }