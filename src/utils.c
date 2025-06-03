
#include "utils.h"

/**
 * keep in mind tokens is declared as `char* hookTokens[3]`
 * 
 * returns the byte offset to this hook entry
 */
uint16_t getTargetHookEntry(char** tokens, char* targetName, int targetNameLen, FILE* confFile) {
    char hookEntry[MAX_HOOK_LENGTH];
    uint16_t line = 0;

    // save the start of teh current line position
    while (fgets(hookEntry, MAX_HOOK_LENGTH, confFile) != NULL) {
        line++;
        if (strlen(hookEntry) < targetNameLen) { continue; }
        if (strncmp(hookEntry, targetName, targetNameLen) != 0) { continue; }
        if (hookEntry[targetNameLen] != '|') { continue; }

        // rewind the file ptr to the start of the target line
        fseek(confFile, 0, SEEK_SET);
        ERR_CODE ret = tokeniseHookEntry(tokens, hookEntry);
        if (ret != ERR_SUCCESS) { return 0; }
        return line;  // return the curretn position of the hook!
    }
    return 0;
}

ERR_CODE tokeniseHookEntry(char** tokens, char* hookEntry) {  
    // perform tokenisation
    int i = 0;
    char* saveState;
    char* token = strtok_r(hookEntry, "|", &saveState);
    while (token != NULL && i < 3) {
        tokens[i++] = token;
        token = strtok_r(NULL, "|", &saveState);
    }
    if (i != 3) { return ERR_INVALID_HOOKED_PATH; }

    // update the description string
    size_t descrLen = strlen(tokens[2]);
    if (descrLen > 0) {
        tokens[2][descrLen - 1] = '\0'; // remove the newline
    } else {
        tokens[2] = "No hook description.";
    }
    return ERR_SUCCESS;
}

void safeFileClose(FILE** file) {
    if (file != NULL && *file != NULL) {
        fclose(*file);
        *file = NULL;
    }
}

char* retrieveErrMsg(ERR_CODE ec) {
    switch (ec) {
        case ERR_SUCCESS: return "Success.";
        case ERR_FAILURE: return "Something failed, go debug it properly smh.";
        case ERR_NULL: return "NULL deteected. Graccceeeffulllyyyy exiting...";
        case ERR_INVALID_CMD: return "Invalid command detected!";
        case ERR_PATH_TO_LONG: return "Provided path longer than 2048 characters! Seems a bit excessive?!";
        case ERR_INVALID_PATH: return "Invalid path format provided, please check it!";
        case ERR_INVALID_HOOKED_PATH: return "Hooked path has been corrupted, update it with `mod` or manually in jumper.conf!";
        default: return "Default case, ERR msg not implemented.";
    }
}