#include "./src/mightymorphinnosql.h"

/*
    Version 0.1.3 Milestone:
    - Create databases
    - Insert and save documents
    - Search documents by Id as fast as you can
    - [Bonus] Search by column values (indexing columns?)

    TOP PRIORITY:
    - Select columns and search by columns
    - Renaming a lot of shit! Do some refactoring for god sake!
    - Fix warning!
    - Json validator

    NEXT:
    - CRUD operations
    - Use a b-tree to store and search indexes?
    - Indexing searching (search by a param)

    TODO BACKLOG:
    - Concurrency anyone?
    - Tcp Layer
    - UI html
    - Load indexes in memory RAM
    - Do we need a query language?
    - InMemory
*/

int main() {
  mm_log("Welcome to Mighty Morphin NoSQL!");
  mm_log("Version: 0.1.3");
  char input[256];

  while (1) {
    printf(">> ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
      mm_log("exit...");
      break;
    }

    input[strcspn(input, "\n")] = 0;
    if (strlen(input) == 0) {
      continue;
    }

    command_run(input);
  }

  return 0;
}