#include "src/commands/command.h"
#include "src/commands/command_keys.h"
#include "src/commands/input_parser.h"
#include "src/core/database/database.h"
#include "src/core/database/database_manager.h"
#include "src/core/inserting/insert.h"
#include "src/core/querying/find.h"
#include "src/utils/utils.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    TOP PRIORITY:
    - CRUD operations
    - Use a b-tree to store and search indexes?
    - Indexing searching (search by a param)
    - Create a front-end for command line

    TODO:
    9. Concurrency anyone?
    11. Use multiple files for indexes and data

    TODO INDEXING:
    3. Load indexes in memory RAM

    TODO QUERY LANGUAGE:
    1. Do we need a query language?

    TCP Layer:
    - Language choose
*/

int main() {
  printf("Welcome to Mighty Morphin NoSQL!\n");
  printf("Version: 0.1.1\n");
  char input[256];
  // char *context = NULL;

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
        Table *table = open_database_table_connection(current_database,
                                                      command->params[0]);
        int id = atoi(command->params[2]);
        char *content = get_by_id(table, id);
        printf("Row found with Id: %d, content: %s\n", id, content);
        free(content);
        close_database_table_connection(table);
      }

      free_command(command);
    }
  }

  return 0;
}