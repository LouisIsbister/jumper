
#include "utils.h"

#ifndef JUMPER_H
#define JUMPER_H

typedef struct {
    char* hName;
    char* hPath;
    char* hDescr;
    char* action;
} HookEntry;

// each time the program is run, only one hook will be targeted
typedef struct {
    char* configPath;

    FILE* confFile;
    HookEntry* hookEntry;
} ProgramContext;

extern ProgramContext* pctx;

#endif
