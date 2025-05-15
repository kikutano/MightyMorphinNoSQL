#include "./tests/unittests.h"
#include <stdlib.h>

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
    - Load indexes in memory RAM
    - Do we need a query language?
*/

int main() {
  printf("Welcome to Mighty Morphin NoSQL!\n");
  printf("Version: 0.1.3\n");
  char input[256];

  while (1) {
    printf(">> ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
      printf("\nexit...\n");
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