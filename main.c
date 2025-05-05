#include "src/commands/command.h"
#include "src/commands/command_keys.h"
#include "src/commands/input_parser.h"
#include "src/core/database/database.h"
#include "src/core/database/database_manager.h"
#include "src/core/database/document.h"
#include "src/core/inserting/insert.h"
#include "src/core/querying/query.h"
#include "src/utils/utils.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    TOP PRIORITY:
    - Return content inside the query
    - Renaming a lot of shit! Do some refactoring for god sake!
    - Fix warning!

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
  printf("Version: 0.1.1\n");
  char input[256];

  Database *current_database;

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

    Command *command = input_parse(input);

    if (command != NULL) {
      if (command->command_id == COMMAND_CREATE_DATABASE) {
        create_database(command->params[0]);
      } else if (command->command_id == COMMAND_CREATE_TABLE) {
        create_database_table(current_database, command->params[0]);
      } else if (command->command_id == COMMAND_OPEN_DATABASE_CONNECTION) {
        current_database = open_database_connection(command->params[0]);
      } else if (command->command_id == COMMAND_INSERT_INTO_TABLE) {
        Table *table = open_database_table_connection(current_database,
                                                      command->params[0]);
        int id = atoi(command->params[1]);
        insert(table, id, command->params[2]);
        close_database_table_connection(table);
      } else if (command->command_id == COMMAND_SELECT) {
        DocumentCollection *collection =
            perform_select(current_database, command);

        print_query_result(collection);

        free(collection);
      }

      free_command(command);
    }
  }

  return 0;
}