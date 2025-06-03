
#include "utils.h"

#ifndef JUMPER_H
#define JUMPER_H

typedef enum {
    JUMP,
    ADD, MOD, DEL,
    DESCR, LIST, HELP
} ACTION;

typedef struct {
    char* hName;
    char* hPath;
    char* hDescr;
    ACTION action;
} HookEntry;

// each time the program is run, only one hook will be targeted
typedef struct {
    char* confPath;
    char* requConfAccess;
    FILE* confFile;

    HookEntry* hookEntry;
} ProgramContext;

#endif
