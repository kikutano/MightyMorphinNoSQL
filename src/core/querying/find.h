#ifndef FIND_H_
#define FIND_H_

#include <stdint.h>
#include <stdio.h>
#include "../database/database.h"
#include "../database/table.h"
#include "../database/document.h"
#include "../../commands/command.h"

uint64_t find_offset_by_id(Table *table, uint32_t target_id);
char *get_by_id(Table *table, uint32_t target_id);
DocumentCollection *perform_select(Database *database, Command *query);

#endif