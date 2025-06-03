#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "jumper.h"
#include "commands.h"


ProgramContext* initProgramContext();
HookEntry* initHookEntry();
void cleanupProgramContext(ProgramContext* pctx);
void printErrCleanupAndExit(ProgramContext* pctx, ERR_CODE ec);

ERR_CODE buildCommand(int argc, char** argv, ProgramContext* pctx);
ERR_CODE openConfigFile(ProgramContext* pctx);
ERR_CODE dispatchExecution(ProgramContext* pctx);
void getTargetedHook(char* hookEntry, char* targetName);

int main(int argc, char** argv) {
    ProgramContext* pctx = initProgramContext();
    if (pctx == NULL) { 
        fprintf(stderr, "Failed to initialize program context\n");
        exit(EXIT_FAILURE);
    }

    ERR_CODE ret = buildCommand(argc, argv, pctx);
    if (ret != ERR_SUCCESS) {
        printErrCleanupAndExit(pctx, ret);
    }

    ret = openConfigFile(pctx);
    if (ret != ERR_SUCCESS) {
        printErrCleanupAndExit(pctx, ret);
    }

    ret = dispatchExecution(pctx);
    
    cleanupProgramContext(pctx);
    
    return 0;
}

// ProgramContext initialisation and cleanup functions  

ProgramContext* initProgramContext() {
    ProgramContext* pctx = malloc(sizeof(ProgramContext));
    if (pctx == NULL) { return NULL; }

    pctx->confPath = "jumper.conf";
    pctx->requConfAccess = "r";
    pctx->confFile = NULL;

    pctx->hookEntry = initHookEntry();
    if (pctx->hookEntry == NULL) { printErrCleanupAndExit(pctx, ERR_NULL); }

    return pctx;
}

HookEntry* initHookEntry() {
    HookEntry* hookEntry = calloc(1, sizeof(HookEntry));
    if (hookEntry == NULL) { return NULL; }
    return hookEntry;
}

void cleanupProgramContext(ProgramContext* pctx) {
    if (pctx == NULL) return;
    if (pctx->hookEntry) free(pctx->hookEntry);
    if (pctx->confFile)  fclose(pctx->confFile);
    free(pctx);
}

void printErrCleanupAndExit(ProgramContext* pctx, ERR_CODE ec) {
    printf("Err: '%s'\n", retrieveErrMsg(ec));
    cleanupProgramContext(pctx);
    exit(1);
}


// command execution functions

ERR_CODE buildCommand(int argc, char** argv, ProgramContext* pctx) {
    HookEntry* hEntry = pctx->hookEntry;
    switch (argc) {
        case 2:
            if (strcmp("-list", argv[1]) == 0 || strcmp("-help", argv[1]) == 0) {
                hEntry->action = strcmp("-list", argv[1]) == 0 ? LIST : HELP;
                return ERR_SUCCESS;
            }
            return ERR_INVALID_CMD;
        case 3:
            hEntry->hName = argv[2];
            if (strcmp("-del", argv[1]) == 0) {
                pctx->requConfAccess = "r+";
                hEntry->action = DEL;
                return ERR_SUCCESS;
            } else if (strcmp("-descr", argv[1]) == 0) {
                hEntry->action = DESCR;
                return ERR_SUCCESS;
            }
            return ERR_INVALID_CMD;
    }

    // build the command for -add and -mod
    char** ptr = argv + 1;
    char* cmd; char* cmdContent;

    pctx->requConfAccess = "r+";
    while ((cmd = *ptr) != NULL && (cmdContent = *(ptr+1)) != NULL) {
        if (strcmp("-add", cmd) == 0) {
            hEntry->action = ADD;
            hEntry->hName = cmdContent;
            pctx->requConfAccess = "a";
        } else if (strcmp("-mod", cmd) == 0) {
            hEntry->action = MOD;
            hEntry->hName = cmdContent;
        } else if (strcmp("-dir", cmd) == 0) {
            hEntry->hPath = cmdContent;
        } else if (strcmp("-descr", cmd) == 0) {
            hEntry->hDescr = cmdContent;
        } else {
            return ERR_INVALID_CMD;
        }
        ptr += 2;
    }
    
    return ERR_SUCCESS;
}

ERR_CODE openConfigFile(ProgramContext* pctx) {
    pctx->confFile = fopen(pctx->confPath, pctx->requConfAccess);
    if (pctx->confFile == NULL) {
        return ERR_FAILURE;
    }
    return ERR_SUCCESS;
}

ERR_CODE dispatchExecution(ProgramContext* pctx) {
    HookEntry* hEntry = pctx->hookEntry;
    char* hookTokens[3];

    // inserts the tokens in the hookTokens pointer
    uint16_t line = getTargetHookEntry(hookTokens, hEntry->hName, strlen(hEntry->hName), pctx->confFile);
    if (line == 0) { return ERR_INVALID_CMD; }

    switch (hEntry->action) {
        case JUMP:;
        case ADD:;
        case MOD:   mod(hEntry, line, &pctx->confFile); return ERR_SUCCESS;
        case DEL:   del(line, &pctx->confFile);         return ERR_SUCCESS;
        case DESCR: descr(hEntry, hookTokens[2]);       return ERR_SUCCESS;
        case LIST:  list(pctx->confFile);               return ERR_SUCCESS;
        case HELP:  help();                             return ERR_SUCCESS;
    }

    return ERR_SUCCESS;
}


void getTargetedHook(char* hookEntry, char* targetName) {
    size_t targetLen = sizeof(*targetName);

    char* saveState;
    char* line = strtok_r(hookEntry, "|", &saveState);

    printf("token: %s\n", line);

}




