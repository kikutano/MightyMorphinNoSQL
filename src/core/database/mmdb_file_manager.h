#ifndef MMDB_FILE_MANAGER_H
#define MMDB_FILE_MANAGER_H
#include <stdio.h>

const char *mmdb_create_database_file_name(const char *db_name);
const char *mmdb_create_database_table_file_name(const char *db_name,
                                                 const char *table_name);
const char *
mmdb_create_database_table_metadata_file_name(const char *db_name,
                                              const char *table_name);
const char *
mmdb_create_database_table_ids_indexes_file_name(const char *db_name,
                                                 const char *table_name);
void mmdb_free_file_name(const char *file_name);
FILE *mmdb_get_database_file(const char *db_name);
FILE *mmdb_get_database_table_file(const char *db_name, const char *table_name);
FILE *mmdb_get_database_table_metadata_file(const char *db_name,
                                            const char *table_name);
FILE *mmdb_get_database_table_ids_indexes_file(const char *db_name,
                                               const char *table_name);
void mmdb_create_database_file(const char *db_name);
void mmdb_create_database_table_file(const char *db_name,
                                     const char *table_name);
void mmdb_create_database_table_metadata_file(const char *db_name,
                                              const char *table_name);
void mmdb_create_database_table_ids_indexes_file(const char *db_name,
                                                 const char *table_name);
void mmdb_delete_database_file(const char *db_name);
void mmdb_delete_database_table_file(const char *db_name,
                                     const char *table_name);
void mmdb_delete_database_table_metadata_file(const char *db_name,
                                              const char *table_name);
void mmdb_delete_database_table_ids_indexes_metadata_file(
    const char *db_name, const char *table_name);
#endif