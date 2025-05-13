#include "src/mightymorphinnosql.h"

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
        free_document_collection(collection);
      }

      free_command(command);
    }
  }

  return 0;
}