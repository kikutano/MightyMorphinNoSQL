#include "input_parser.h"
#include "command_keys.h"
#include "token_keys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Command *input_parse(char *input) {
  char *context = NULL;

  char *token = strtok_s(input, " ", &context);

  if (strcmp(token, CREATE) == 0) {
    token = strtok_s(NULL, " ", &context);

    if (token != NULL && strcmp(token, DATABASE) == 0) {
      token = strtok_s(NULL, " ", &context);

      if (token == NULL) {
        printf("Error. Database name must be specified!\n");
        return NULL;
      }

      Command *command = malloc(sizeof(Command));
      command->command_id = COMMAND_CREATE_DATABASE;
      command->params_count = 1;
      command->params = NULL;
      command->params = malloc(sizeof(char *));
      command->params[0] = malloc(strlen(token) + 1);
      strcpy_s(command->params[0], sizeof(strlen(token) + 1), token);

      return command;
    } else if (token != NULL && strcmp(token, TABLE) == 0) {
      token = strtok_s(NULL, " ", &context);
      if (token == NULL) {
        printf("Error. Table name must be specified!\n");
        return NULL;
      }

      Command *command = malloc(sizeof(Command));
      command->command_id = COMMAND_CREATE_TABLE;
      command->params_count = 1;
      command->params = NULL;
      command->params = malloc(sizeof(char *));
      command->params[0] = malloc(strlen(token) + 1);
      strcpy_s(command->params[0], sizeof(strlen(token) + 1), token);

      return command;
    }
  } else if (strcmp(token, OPEN) == 0) {
    token = strtok_s(NULL, " ", &context);

    if (token != NULL && strcmp(token, CONNECTION) == 0) {
      token = strtok_s(NULL, " ", &context);

      if (token != NULL && strcmp(token, DATABASE) == 0) {
        token = strtok_s(NULL, " ", &context);
        if (token == NULL) {
          printf("Error. Database name must be specified!\n");
          return NULL;
        }

        Command *command = malloc(sizeof(Command));
        command->command_id = COMMAND_OPEN_DATABASE_CONNECTION;
        command->params_count = 1;
        command->params = NULL;
        command->params = malloc(sizeof(char *));
        command->params[0] = malloc(strlen(token) + 1);
        strcpy_s(command->params[0], sizeof(strlen(token) + 1), token);

        return command;
      }
    }
  }

  return NULL;
}

void free_command(Command *command) {
  if (command != NULL) {
    for (int i = 0; i < command->params_count; i++) {
      free(command->params[i]);
    }

    free(command->params);
    free(command);
  }
}