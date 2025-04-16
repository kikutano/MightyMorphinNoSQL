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
  printf("Version: 0.1.0\n");
  char input[256];
  char *context = NULL;

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
      }

      free_command(command);
    }

    /*char *token = strtok_s(input, " ", &context);

    // create database [name]
    // create table [name]
    if (strcmp(token, "create") == 0) {
      token = strtok_s(NULL, " ", &context);

      if (token != NULL && strcmp(token, "database") == 0) {
        token = strtok_s(NULL, " ", &context);

        if (token == NULL) {
          printf("Error. Database name must be specified!\n");
          continue;
        } else {
          create_database(token);
        }
      } else if (token != NULL && strcmp(token, "table") == 0) {
        token = strtok_s(NULL, " ", &context);
        if (current_database != NULL) {
          if (token == NULL) {
            printf("Error. Table name must be specified!\n");
            continue;
          } else {
            create_database_table(current_database, token);
          }
        } else {
          printf("Error. Use open connection database [name] first!\n");
        }
      }
    }
    // open connection database [name]
    else if (strcmp(token, "open") == 0) {
      token = strtok_s(NULL, " ", &context);

      if (token != NULL && strcmp(token, "connection") == 0) {
        token = strtok_s(NULL, " ", &context);

        if (strcmp(token, "database") == 0) {
          token = strtok_s(NULL, " ", &context);
          current_database = open_database_connection(token);
        }
      }
    }
    // insert into [table] [id] [content]
    else if (strcmp(token, "insert") == 0) {
      token = strtok_s(NULL, " ", &context);

      if (token != NULL && strcmp(token, "into") == 0) {
        token = strtok_s(NULL, " ", &context);
        Table *table = open_database_table_connection(current_database, token);
        token = strtok_s(NULL, " ", &context);
        int id = atoi(token);
        token = strtok_s(NULL, " ", &context);
        insert(table, id, token);
        close_database_table_connection(table);
      }
    }
    // find on [table] by [id]
    else if (strcmp(token, "find") == 0) {
      token = strtok_s(NULL, " ", &context);

      if (token != NULL && strcmp(token, "on") == 0) {
        token = strtok_s(NULL, " ", &context);
        Table *table = open_database_table_connection(current_database, token);
        token = strtok_s(NULL, " ", &context);
        int id = atoi(token);
        char *content = get_by_id(table, id);
        printf("Row found with Id: %d, content: %s\n", id, content);
        free(content);
        close_database_table_connection(table);
      }
    }
    // print database [name]
    // print table [name]
    else if (strcmp(token, "print") == 0) {
      token = strtok_s(NULL, " ", &context);

      if (token != NULL && strcmp(token, "database") == 0) {
        token = strtok_s(NULL, " ", &context);
        print_database_metadata(token);
      } else if (token != NULL && strcmp(token, "table") == 0) {
        token = strtok_s(NULL, " ", &context);
        print_database_table(token);
      }
    }*/
  }

  return 0;
}