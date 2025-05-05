#ifndef QUERY_H_
#define QUERY_H_

#include <stdint.h>
#include <stdio.h>
#include "../database/database.h"
#include "../database/table.h"
#include "../database/document.h"
#include "../../commands/command.h"

DocumentCollection *perform_select(Database *database, Command *query);

#endif