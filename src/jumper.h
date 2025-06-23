
#include "utils.h"

#ifndef JUMPER_H
#define JUMPER_H

typedef enum {
    JUMP,
    ADD, MOD, DEL,
    DESCR, LIST, HELP,
    NOTHING
} ACTION;

// each time the program is run, only one hook will be targeted
typedef struct {
    char *confPath;
    char *requiredConfAccess;
    FILE *confFilePtr;
    
    ACTION action;
    char *hookName;

    char *newHookDir;
    char *newHookDescr;
} ProgramContext;

#endif
