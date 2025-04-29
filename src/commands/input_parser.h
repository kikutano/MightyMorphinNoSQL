#ifndef INPUT_PARSER_H_
#define INPUT_PARSER_H_

#include "Command.h"

Command* input_parse(char *input);
void free_command(Command *command);

#endif

/*
Supported commands:
* [] variable value

Queryies:
- select * from [table_name] where [column] = [value] --column id only ;)
- select * from [table_name]

Commands:

Commands, rows:
- insert into [table] [id] [content]

Commands, database:
- create database [name]
- create table [name]
- open connection database [name]
 */