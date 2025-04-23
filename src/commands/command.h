#ifndef COMMAND_H_
#define COMMAND_H_

typedef struct {
    unsigned int command_id;
    unsigned int params_count;
    char **params;
} Command;

#endif