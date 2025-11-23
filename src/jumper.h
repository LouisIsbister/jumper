
#include "utils.h"

#ifndef JUMPER_H
#define JUMPER_H

typedef enum {
        JUMP, ADD, MOD, DEL,
        DESCR, LIST, HELP,
        NOTHING
} ACTION;

typedef struct ARG {
        char* arg;
        char* value;
} argument;

typedef struct JUMPER_CONTEXT_STRUCT {
        uint8_t arg_count;
        argument* args;

        char required_conf_access[3];

        ACTION action;
} jumper_context;


#endif
