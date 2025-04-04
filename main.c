#include "src/core/database/database_manager.h"
#include "src/utils/utils.h"
// #include "src/core/inserting/insert.h"
// #include "src/core/querying/find.h"
// #include "src/utils/utils.h"
#include <limits.h>
#include <stdint.h>

/*
        WORKING ON:
        4. Can we remove \n from file?
        2. Indexing searching (search by a param)

        TODO:
        6. memcpy VS fprintf
        7. Files are not db, but entities!
        8. CRUD operations
        9. Concurrency anyone?
        10. Console table visualizator
        11. Use multiple files for indexes and data
        12. Use a metadata file

        INDEXING:
        1. Use a b-tree to store and search indexes?
        3. Load indexes in memory RAM

        QUERY LANGUAGE:
        1. Do we need a query language?
*/

int main() {

  create_database("mydb");
  Database *db = open_database_connection("mydb");
  create_database_table(db, "users");
  create_database_table(db, "products");
  close_database_connection(db);

  // do some inserts
  // do some queries

  print_database_metadata("mydb.db");

  return 0;
}