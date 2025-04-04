#ifndef DATABASE_MANAGER_H_
#define DATABASE_MANAGER_H_
#include "database.h"
#include "table.h"
#include <stdio.h>

void create_database(const char *name);
Database *open_database_connection(const char *name);
void close_database_connection(Database *database);
void create_database_table(Database *database, const char *name);
Table *get_database_table(Database *database, const char *name);

#endif