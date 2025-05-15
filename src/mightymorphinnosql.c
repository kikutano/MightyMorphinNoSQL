#include "mightymorphinnosql.h"
#include "commands/command_keys.h"
#include "core/database/database_manager.h"
#include <stdlib.h>

Database *current_database;

void command_run(const char *input) {
  Command *command = input_parse(input);

  if (command != NULL) {
    if (command->command_id == COMMAND_CREATE_DATABASE) {
      create_database(command->params[0]);
    } else if (command->command_id == COMMAND_DELETE_DATABASE) {
      delete_database(command->params[0]);
    } else if (command->command_id == COMMAND_DELETE_TABLE) {
      mm_log("command not supported yet!");
    } else if (command->command_id == COMMAND_CREATE_TABLE) {
      create_database_table(current_database, command->params[0]);
    } else if (command->command_id == COMMAND_OPEN_DATABASE_CONNECTION) {
      current_database = open_database_connection(command->params[0]);
    } else if (command->command_id == COMMAND_INSERT_INTO_TABLE) {
      Table *table =
          open_database_table_connection(current_database, command->params[0]);
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