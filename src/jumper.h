
#include "utils.h"

#ifndef JUMPER_H
#define JUMPER_H

typedef struct jmp_context jmp_context_t;
typedef struct jmp_arg jmp_arg_t;
typedef struct jmp_flag_context jmp_flag_context_t;


#define DEBUG_JUMPER_CONTEXT(jctx)                                                              \
        printf("\nArg count: %d\n", (jctx)->arg_count);                                         \
        printf("Action: '%s'\n", flag_type_to_str((jctx)->most_significant_flag));              \
        for (int i = 0; i < (jctx)->arg_count; i++) {                                           \
                jmp_arg_t* a = (jctx)->args[i];                                                  \
                printf(" %d) %s ==> %s\n", i, flag_type_to_str(a->flag->type), a->value);       \
        }



typedef struct jmp_context {
        uint8_t arg_count;
        jmp_arg_t* args[MAX_ARG_COUNT];
        jmp_flag_t most_significant_flag;
} jmp_context_t;

typedef struct jmp_arg {
        const jmp_flag_context_t* flag;
        char* value;
} jmp_arg_t;

typedef struct jmp_flag_context {
        jmp_flag_t type;
        const char* requ_config_file_permissions;
        const uint8_t combinations[4];
} jmp_flag_context_t;




#endif
