
#include "utils.h"
#include <string.h>

/**
 * returns the byte offset to this hook entry
 */
uint16_t 
retrieveHook(char *targetName, char *hookBuffer, FILE *confFile) {
        size_t targetNameLen = strlen(targetName);
        
        uint16_t line = 0;
        char hookEntry[MAX_HOOK_LENGTH];
        while (fgets(hookEntry, MAX_HOOK_LENGTH, confFile) != NULL) {
                line++;

                if (strlen(hookEntry) < targetNameLen) continue;
                if (strncmp(hookEntry, targetName, targetNameLen) != 0) continue;
                if (hookEntry[targetNameLen] != '|') continue;  // ensure we havent just found a prefix

                strncpy(hookBuffer, hookEntry, MAX_HOOK_LENGTH);
                // rewind the file ptr to the beginning
                fseek(confFile, 0, SEEK_SET);
                return line;
        }
        return 0;
}


ERR_CODE 
tokeniseHookEntry(char **tokens, char *hookEntry) {  
        int i = 0;

        // perform tokenisation
        char *saveState;
        char *token = strtok_r(hookEntry, "|", &saveState);
        while (token != NULL && i < 3) {
                tokens[i++] = token;
                token = strtok_r(NULL, "|", &saveState);
        }
        if (i != 3) { return ERR_INVALID_HOOKED_PATH; }

        return ERR_SUCCESS;
}

void 
formatHook(char *hookBuffer, char *name, char *dir, char *descr) {
        snprintf(hookBuffer, MAX_HOOK_LENGTH, "%s|%s|%s\n", name, dir, descr);
}

void 
formatFromTokens(char *hookBuffer, char **tokens) {
        snprintf(hookBuffer, MAX_HOOK_LENGTH, "%s|%s|%s\n", tokens[0], tokens[1], tokens[2]);
}

void 
safeFileClose(FILE **file) {
        if (file != NULL && *file != NULL) {
                fclose(*file);
                *file = NULL;
        }
}

char*
retrieveErrMsg(ERR_CODE ec) {
        switch (ec) {
                case ERR_SUCCESS: return "Success.";
                case ERR_FAILURE: return "Something failed, go debug it properly smh.";
                case ERR_NULL: return "NULL deteected. Graccceeeffulllyyyy exiting...";
                case ERR_INVALID_CMD: return "Invalid command detected!";
                case ERR_PATH_TO_LONG: return "Provided path longer than 2048 characters! Seems a bit excessive?!";
                case ERR_INVALID_PATH: return "Invalid path format provided, please check it!";
                case ERR_UNKNOWN_HOOK: return "Unknown hook detected!";
                case ERR_INVALID_HOOKED_PATH: return "Hooked path has been corrupted, update it with `mod` or manually in jumper.conf!";
                default: return "Default case, ERR msg not implemented.";
        }
}