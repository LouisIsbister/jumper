
#include "utils.h"

#ifndef JUMPER_H
#define JUMPER_H


#define MAX_ARGS                3

#define DEBUG_JUMPER_CONTEXT(jctx)                                              \
        printf("Arg count: %d\n", (jctx)->arg_count);                           \
        for (int i = 0; i < (jctx)->arg_count; i++) {                           \
                argument* a = (jctx)->args[i];                                  \
                printf(" %d) %s ==> %s\n", i, a->arg, a->value);                \
        }

typedef struct ARG argument;
typedef struct JUMPER_CONTEXT_STRUCT jumper_context;

typedef struct JUMPER_CONTEXT_STRUCT {
        uint8_t arg_count;
        argument* args[MAX_ARGS];

        char required_conf_access[3];
} jumper_context;

typedef struct ARG {
        char* arg;
        char* value;
} argument;


#endif
