#include "input_parser.h"
#include "command.h"
#include "command_keys.h"
#include "token_keys.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vadefs.h>

Command *create_command(int command_id, int params_count, ...);
Command *input_parse(char *input);
Command *parse_create(char *context);
Command *parse_open(char *context);
Command *parse_insert(char *context);
Command *parse_select(char *context);

void free_command(Command *command) {
  if (command != NULL) {
    for (int i = 0; i < command->params_count; i++) {
      free(command->params[i]);
    }

    free(command->params);
    free(command);
  }
}

Command *create_command(int command_id, int params_count, ...) {
  Command *command = malloc(sizeof(Command));
  command->command_id = command_id;
  command->params_count = params_count;
  command->params = malloc(sizeof(char *) * params_count);

  va_list args;
  va_start(args, params_count);

  for (int i = 0; i < params_count; i++) {
    const char *param = va_arg(args, const char *);

    if (param == NULL) {
      printf("Error during params reading. Param cannot be null!");
      free_command(command);
      return NULL;
    }

    command->params[i] = malloc(strlen(param) + 1);
    strcpy_s(command->params[i], strlen(param) + 1, param);
  }

  return command;
}

char *get_next_token(char **context) {
  return strtok_s(NULL, DELIMETER, context);
}

int is_valid_token(const char *token, const char *token_key) {
  if (token == NULL || strcmp(token, CONNECTION) != 0)
    return 0;
  return 1;
}

Command *input_parse(char *input) {
  char *context = NULL;
  char *token = strtok_s(input, DELIMETER, &context);

  if (token == NULL)
    return NULL;

  if (strcmp(token, CREATE) == 0) {
    return parse_create(context);
  } else if (strcmp(token, OPEN) == 0) {
    return parse_open(context);
  } else if (strcmp(token, INSERT) == 0) {
    return parse_insert(context);
  } else if (strcmp(token, SELECT) == 0) {
    return parse_select(context);
  }

  return NULL;
}

// Parses all create commands.
Command *parse_create(char *context) {
  char *token = strtok_s(NULL, DELIMETER, &context);
  if (token == NULL)
    return NULL;

  if (strcmp(token, DATABASE) == 0) {
    // create database [name]
    char *db_name = strtok_s(NULL, DELIMETER, &context);
    if (db_name == NULL) {
      printf("Error. Database name must be specified!\n");
      return NULL;
    }
    return create_command(COMMAND_CREATE_DATABASE, 1, db_name);
  } else if (strcmp(token, TABLE) == 0) {
    // create table [name]
    char *table_name = strtok_s(NULL, DELIMETER, &context);
    if (table_name == NULL) {
      printf("Error. Table name must be specified!\n");
      return NULL;
    }
    return create_command(COMMAND_CREATE_TABLE, 1, table_name);
  }

  return NULL;
}

// Parse all open commands
Command *parse_open(char *context) {
  // connection
  char *token = strtok_s(NULL, DELIMETER, &context);
  if (token == NULL || strcmp(token, CONNECTION) != 0)
    return NULL;

  // database
  token = strtok_s(NULL, DELIMETER, &context);
  if (token == NULL || strcmp(token, DATABASE) != 0)
    return NULL;

  // [name]
  char *db_name = strtok_s(NULL, DELIMETER, &context);
  if (db_name == NULL) {
    printf("Error. Database name must be specified!\n");
    return NULL;
  }

  return create_command(COMMAND_OPEN_DATABASE_CONNECTION, 1, db_name);
}

// Parse all insert commands
Command *parse_insert(char *context) {
  // into
  char *token = strtok_s(NULL, DELIMETER, &context);
  if (token == NULL || strcmp(token, INTO) != 0)
    return NULL;

  // [table]
  char *table_name = strtok_s(NULL, DELIMETER, &context);
  if (table_name == NULL) {
    printf("Error. Table name must be specified!\n");
    return NULL;
  }

  // [id]
  char *id = strtok_s(NULL, DELIMETER, &context);

  // [content]
  char *content = strtok_s(NULL, DELIMETER, &context);

  // insert into [table] [id] [content]
  Command *command =
      create_command(COMMAND_INSERT_INTO_TABLE, 3, table_name, id, content);

  return command;
}

/*
  - Original format:
    select * from [table_name] where [column] = [value] --column id only ;)
  - Parsed format:
    [table_name][column][value]
  - Todo:
  - Column selection is not supported. Only select * are allowed for now.
  - Where clause demandatory.
*/
Command *parse_select(char *context) {
  //*
  char *token_star = get_next_token(&context);
  if (is_valid_token(token_star, STAR)) {
    printf("Sintax error. Only select * allowed.");
    return NULL;
  }

  // from
  char *token_from = get_next_token(&context);
  if (is_valid_token(token_from, FROM)) {
    printf("Sintax error. Only from after * allowed.");
    return NULL;
  }

  //[table_name]
  char *token_table_name = get_next_token(&context);
  if (token_table_name == NULL) {
    printf("Error. Table name must be specified!\n");
    return NULL;
  }

  // where
  char *token_where = get_next_token(&context);
  if (is_valid_token(token_where, WHERE)) {
    printf("Sintax error. Where is the where after the table name?!");
    return NULL;
  }

  // [column]
  char *token_column = get_next_token(&context);
  if (token_column == NULL || strcmp(token_column, "id") != 0) {
    printf("Sintax error. Only id can be used as where clause.");
    return NULL;
  }

  // =
  char *token_equal = get_next_token(&context);
  if (is_valid_token(token_equal, EQUAL)) {
    printf("Sintax error. Where is = after the id?");
    return NULL;
  }

  // [value]
  char *token_value = get_next_token(&context);
  if (token_value == NULL) {
    printf("Sintax error. Value must be specified.");
    return NULL;
  }

  return create_command(COMMAND_SELECT, 3, token_table_name, token_column,
                        token_value);
}
