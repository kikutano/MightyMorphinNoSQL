#include "src/core/database/database_manager.h"
#include "src/core/inserting/insert.h"
#include "src/utils/utils.h"
#include "src/core/querying/find.h"

/*
    WORKING ON:
    11. Command line for create and insert things
    2. Indexing searching (search by a param)
    10. Console table visualizator

    TODO:
    6. memcpy VS fprintf
    8. CRUD operations
    9. Concurrency anyone?
    11. Use multiple files for indexes and data

    TODO INDEXING:
    1. Use a b-tree to store and search indexes?
    3. Load indexes in memory RAM

    TODO QUERY LANGUAGE:
    1. Do we need a query language?

    TCP Layer:
    - Language choose
*/

int main() {
  create_database("mydb");
  Database *db = open_database_connection("mydb");
  create_database_table(db, "users");
  create_database_table(db, "products");

  Table *users_table = open_database_table_connection(db, "users");
  insert(users_table, 1, "Dutch Matrix");
  insert(users_table, 2, "John Rambo");
  insert(users_table, 3, "Martin Riggs");
  insert(users_table, 4, "Roger Murtaugh");

  Table *products_table = open_database_table_connection(db, "products");
  insert(products_table, 1, "Ratios");
  insert(products_table, 2, "Meats");
  insert(products_table, 3, "Weapons");

  char *users_content = get_by_id(users_table, 3);
  char *products_content = get_by_id(products_table, 2);

  printf("> Row user found with Id: 3, content: %s\n", users_content);
  printf("> Row product found with Id: 2, content: %s\n", products_content);

  free(users_content);
  free(products_content);

  close_database_table_connection(users_table);
  close_database_table_connection(products_table);

  close_database_connection(db);

  print_database_metadata("mydb.db");
  print_database_table("mydb_users_table.db");
  print_database_table("mydb_products_table.db");

  return 0;
}