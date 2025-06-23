
#include "jumper.h"
#include "commands.h"


ProgramContext *initProgramContext();
void exec(ProgramContext *pctx, int argc, char **argv);

// parse and execute the provided command
ERR_CODE parseCommand(ProgramContext *pctx, int argc, char **argv);
ERR_CODE dispatchExecution(ProgramContext *pctx);

// opening jumper.conf file
ERR_CODE openConfigFile(ProgramContext *pctx);

// program context cleanup functions
void cleanupProgramContext(ProgramContext *pctx);
void printErrCleanupAndExit(ProgramContext *pctx, ERR_CODE ec);


int main(int argc, char **argv) {
    if (argc <= 1) {
        fprintf(stderr, "Incorrect usage of jumper. Please run 'jumper -help' for more information.\n");
        exit(1);
    }

    ProgramContext *pctx;
    pctx = initProgramContext();
    if (pctx == NULL) { 
        fprintf(stderr, "Fatal error, failed to init program context.\n");
        exit(1);
    }

    // execute the program
    exec(pctx, argc, argv);
    // cleanup program content memory 
    cleanupProgramContext(pctx);
    
    return 0;
}


/**
 * @return the new program content structure
 */
ProgramContext *initProgramContext() {
    ProgramContext *pctx = malloc(sizeof(ProgramContext));
    if (pctx == NULL) { return NULL; }

    pctx->confPath = "jumper.conf";
    pctx->requiredConfAccess = "r";
    pctx->confFilePtr = NULL;

    // stored command line arguments
    pctx->action = NOTHING;
    pctx->hookName = "";

    pctx->newHookDir = NULL;
    pctx->newHookDescr = NULL;

    return pctx;
}


void exec(ProgramContext *pctx, int argc, char **argv) {
    if (argc == 2 && strcmp("-init", argv[1]) == 0) {
        // init
        printf("Initing...");
        return;
    }


    ERR_CODE ret;

    ret = parseCommand(pctx, argc, argv);
    if (ret != ERR_SUCCESS) printErrCleanupAndExit(pctx, ret);

    ret = openConfigFile(pctx);
    if (ret != ERR_SUCCESS) printErrCleanupAndExit(pctx, ret);

    ret = dispatchExecution(pctx);
    if (ret != ERR_SUCCESS) printErrCleanupAndExit(pctx, ret);

    cleanupProgramContext(pctx);
}


ERR_CODE tryBuild2ArgCommand(ProgramContext *pctx, char **argv);
ERR_CODE tryBuild3ArgCommand(ProgramContext *pctx, char **argv);

/**
 * @brief given the command line arguments, parse their information into the program context
 * 
 * @param pctx the program context structure whose command info this method is trying to set
 * @param argc the number of args passed
 * @param argv the args themselvs
 * @return the appropriate return code
 */
ERR_CODE parseCommand(ProgramContext *pctx, int argc, char **argv) {
    switch (argc) {
        case 2:
            return tryBuild2ArgCommand(pctx, argv); 
        case 3:
            return tryBuild3ArgCommand(pctx, argv);
    }

    // build the command for -add and -mod
    if (strcmp("-add", argv[1]) == 0) {
        pctx->requiredConfAccess = "a";
        pctx->hookName = argv[2];
        pctx->action = ADD;
    } else if (strcmp("-mod", argv[1]) == 0) {
        pctx->requiredConfAccess = "r+";
        pctx->hookName = argv[2];
        pctx->action = MOD;
    } else {
        printf("Unknown flag: '%s'\n", argv[1]);
        return ERR_INVALID_CMD;
    }

    // consume the rest of the arguments passed after the add and mod flags

    char **ptr = argv + 3; 
    char *flag, *value;
    while ((flag = *ptr) != NULL && (value = *(ptr+1)) != NULL) {
        if (strcmp("-dir", flag) == 0) {
            pctx->newHookDir = value;
        } else if (strcmp("-descr", flag) == 0) {
            pctx->newHookDescr = value;
        } else {
            return ERR_INVALID_CMD;
        }
        ptr += 2;
    }
    
    return ERR_SUCCESS;
}


/** @brief function that attempts to parse the commands -init -list -help and 'jumper <hook_name>'
 *
 * @param argv command line args 
 * @param pctx program context structure
 * @return flag for success/failure
 */
ERR_CODE tryBuild2ArgCommand(ProgramContext *pctx, char **argv) {
    if (strcmp("-list", argv[1]) == 0) {
        pctx->action = LIST;
        return ERR_SUCCESS;
    } else if (strcmp("-help", argv[1]) == 0) {
        pctx->action = HELP;
        return ERR_SUCCESS;
    } else {
        // the case when we are actually jumping!
        pctx->hookName = argv[1]; // the only time argv[1] is not a cmd
        pctx->action = JUMP;
        return ERR_SUCCESS;
    }

    printf("Unknown flag: '%s'\n", argv[1]);
    return ERR_INVALID_CMD;
}

/** @brief function to parse the commands -del and -descr
 *
 * @param argv command line args 
 * @param pctx program context structure
 * @return flag for success/failure
 */
ERR_CODE tryBuild3ArgCommand(ProgramContext *pctx, char **argv) {
    pctx->hookName = argv[2]; // set the provided hook name
    if (strcmp("-del", argv[1]) == 0) {
        pctx->requiredConfAccess = "r+"; // we need tor read and write to delete
        pctx->action = DEL;
        return ERR_SUCCESS;
    } else if (strcmp("-descr", argv[1]) == 0) {
        pctx->action = DESCR;
        return ERR_SUCCESS;
    }

    printf("Unknown flag: '%s'\n", argv[1]);
    return ERR_INVALID_CMD;
}


/**
 * @brief having parsed the command, take the appropriate action to execute the actions
 * 
 * @param pctx the program context structure
 * @return the appropriate return code
 */
ERR_CODE dispatchExecution(ProgramContext *pctx) {
    char hookBuffer[MAX_HOOK_LENGTH];
    char *hookName = pctx->hookName;
    FILE *confFile = pctx->confFilePtr;

    // try and execute commands that do not require the the exact hook
    // to be retrieved from the .conf file!
    switch (pctx->action) {
        case LIST: list(pctx->confFilePtr); return ERR_SUCCESS;
        case HELP: help();                  return ERR_SUCCESS;
        case ADD:
            formatHook(hookBuffer, hookName, pctx->newHookDir, pctx->newHookDescr);
            add(hookBuffer, pctx->confFilePtr);
            return ERR_SUCCESS;
        case DESCR:
            // descr(hookName, hookDescr);
            return ERR_SUCCESS;
        case JUMP:
            return ERR_SUCCESS;
        default: ;
    }

    // get the bytes in the .conf file before the hook,
    // and store the hook itself in hookBuffer
    uint16_t lineNumber = retrieveHook(hookName, hookBuffer, confFile);
    if (lineNumber == 0) {
        return ERR_UNKNOWN_HOOK;
    }

    switch (pctx->action) {
        case MOD:
            mod(lineNumber, hookName, pctx->newHookDir, pctx->newHookDescr, hookBuffer, confFile);
            return ERR_SUCCESS;
        case DEL:
            del(lineNumber, confFile);
            return ERR_SUCCESS;

        default: return ERR_FAILURE;
    }
}



ERR_CODE openConfigFile(ProgramContext *pctx) {
    pctx->confFilePtr = fopen(pctx->confPath, pctx->requiredConfAccess);
    if (pctx->confFilePtr == NULL) {
        return ERR_FAILURE;
    }

    return ERR_SUCCESS;
}


void cleanupProgramContext(ProgramContext *pctx) {
    if (pctx == NULL) return;
    if (pctx->confFilePtr) fclose(pctx->confFilePtr);

    free(pctx);
}

void printErrCleanupAndExit(ProgramContext *pctx, ERR_CODE ec) {
    printf("Err: '%s'\n", retrieveErrMsg(ec));
    cleanupProgramContext(pctx);
    exit(ec);
}

