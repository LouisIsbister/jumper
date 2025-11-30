
#include "utils.h"

#ifndef JUMPER_H
#define JUMPER_H

typedef struct jmp_context jmp_context_t;
typedef struct jmp_arg jmp_arg_t;
typedef struct jmp_flag_context jmp_flag_context_t;


#define DEBUG_JUMPER_CONTEXT(jctx)                                                      \
        printf("\nArg count: %d\n", (jctx)->arg_count);                                 \
        printf("Action: '%s'\n", (jctx)->msf->name);                                    \
        for (int i = 0; i < (jctx)->arg_count; i++) {                                   \
                jmp_arg_t *a = (jctx)->args[i];                                         \
                printf(" %d) %s ==> %s\n", i+1, a->flag->name, a->value);               \
        }



typedef struct jmp_context {
        uint8_t arg_count;
        jmp_arg_t *args[MAX_ARG_COUNT];
        jmp_flag_context_t *msf; // most significant flag
} jmp_context_t;

typedef struct jmp_arg {
        jmp_flag_context_t *flag;
        char *value;
} jmp_arg_t;

typedef struct jmp_flag_context {
        const jmp_flag_t type;
        const char *name;
        const char *file_mode;
        const uint8_t combinations[4];
} jmp_flag_context_t;


#endif
