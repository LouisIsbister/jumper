#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "jumper.h"
#include "commands.h"


ProgramContext* initProgramContext();
HookEntry* initHookEntry();
void cleanupProgramContext(ProgramContext* progCtx);
void printErrCleanupAndExit(ERR_CODE ec);

void dispatchCommand(int argc, char** argv);
void getTargetedHook(char* hookEntry, char* targetName);


ProgramContext* pctx = NULL;


int main(int argc, char** argv) {
    pctx = initProgramContext();
    if (pctx == NULL) { printErrCleanupAndExit(ERR_NULL); }

    pctx->configPath = argv[0];
    ERR_CODE ret = retrieveConfFile(&pctx->configPath);
    if (ret != ERR_SUCCESS) { printErrCleanupAndExit(ret); }
    printf("Testing .conf path bollocks: '%s'\n", pctx->configPath);
    
    FILE* conf;
    pctx->confFile = fopen(pctx->configPath, "r");
    if (pctx->confFile == NULL) { printErrCleanupAndExit(ERR_NULL); }

    // char hookEntry[MAX_HOOK_LENGTH];
    // while (fgets(hookEntry, MAX_HOOK_LENGTH, conf) != NULL) {
    //     // Print the data
    //     printf("'%s'\n", hookEntry);
    // }

    dispatchCommand(argc, argv);

    
    cleanupProgramContext(pctx);
    
    return 0;
}

// ProgramContext initialisation and cleanup functions  

ProgramContext* initProgramContext() {
    ProgramContext* progCtx = malloc(sizeof(ProgramContext));
    if (progCtx == NULL) { return NULL; }
    progCtx->configPath = NULL;

    progCtx->hookEntry = initHookEntry();
    if (progCtx->hookEntry == NULL) { 
        free(progCtx);
        return NULL; 
    }

    progCtx->confFile = NULL;
    return progCtx;
}

HookEntry* initHookEntry() {
    HookEntry* hookEntry = calloc(1, sizeof(HookEntry));
    if (hookEntry == NULL) { return NULL; }
    return hookEntry;
}

void cleanupProgramContext(ProgramContext* progCtx) {
    if (progCtx->hookEntry != NULL)
        free(progCtx->hookEntry);
    if (progCtx->confFile != NULL)
        fclose(progCtx->confFile);
    free(progCtx);
}

void printErrCleanupAndExit(ERR_CODE ec) {
    printf("Err: '%s'\n", retrieveErrMsg(ec));
    cleanupProgramContext(pctx);
    exit(1);
}


// command execution functions

void dispatchCommand(int argc, char** argv) {
    switch (argc) {
        case 2: 
            if (strncmp(LIST, argv[1], 5) == 0) {
                list(pctx->confFile);
            } else if (strncmp(HELP, argv[1], 5) == 0) {
                help();
            } else {
                // been given a hook name, need to jump
            }
        case 3: ;
            if (strncmp(DEL, argv[1], 4) == 0) {
                pctx->hookEntry->action = DEL;
                // delete a given hook
            } else if (strncmp(DESCR, argv[1], 6) == 0) {
                // print the description for a hook
            } else {
                // invalid command
            }
    }

    char** ptr = argv + 1;
    while (*ptr != NULL && *(ptr+1) != NULL) {
        printf("%s %s\n", *ptr, *(ptr+1));
        // handle the action paramater
        // -add
        if (strncmp(ADD, *ptr, 4) == 0) {
            pctx->hookEntry->action = ADD;
            pctx->hookEntry->hName = *(ptr+1);
        } // -mod
        else if (strncmp(MOD, *ptr, 4) == 0) {
            pctx->hookEntry->action = MOD;
            pctx->hookEntry->hName = *(ptr+1);
        }// -dir
        else if (strncmp(DIR, *ptr, 4) == 0) {
            pctx->hookEntry->hPath = *(ptr+1);
        } // -descr
        else if (strncmp(DESCR, *ptr, 6) == 0) {
            pctx->hookEntry->hDescr = *(ptr+1);
        }
        ptr += 2;
    }
}

void getTargetedHook(char* hookEntry, char* targetName) {
    size_t targetLen = sizeof(*targetName);

    char* saveState;
    char* line = strtok_r(hookEntry, "|", &saveState);

    printf("token: %s\n", line);

}




