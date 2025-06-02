
#include "utils.h"


/**
 * the winapi function GetCurrentDirectoryA returns a couple of values,
 * if it returns 0 something failed. However, if the provided buffer is
 * too small, the return vale is the required size of the buffer! Hence
 * why __tmp__ is being passed with length zero to get that required size
 */
ERR_CODE retrieveConfFile(char** path) {
    if (path == NULL || *path == NULL) return ERR_NULL;
    
    // fine the last occurrence of a backslash in the module path
    size_t len = strlen(*path);
    ssize_t i = (ssize_t) len - 1; // go to the last backslash in the path
    while (i >= 0 && (*path)[i] != '\\') {
        i--;
    }
    if (i < 0) { return ERR_INVALID_PATH; }

    size_t newLen = i + CONF_FNAME_LEN;
    char* newPath = realloc(*path, newLen);
    if (newPath == NULL) { return ERR_NULL; }
    
    memcpy(newPath + i, CONF_FNAME, CONF_FNAME_LEN);
    newPath[newLen - 1] = '\0';

    *path = newPath;
    return ERR_SUCCESS;
}

ERR_CODE tokeniseHookEntry(char** tokenBuffer, char* hookEntry) {  
    // perform tokenisation
    int i = 0;
    char* saveState;
    char* line = strtok_r(hookEntry, "|", &saveState);
    while (line != NULL && i < 3) {
        tokenBuffer[i++] = line;
        line = strtok_r(NULL, "|", &saveState);
    }
    if (i != 2) { return ERR_INVALID_HOOKED_PATH; }

    // update the description string
    size_t descrLen = strlen(tokenBuffer[2]);
    if (descrLen > 0) {
        tokenBuffer[2][descrLen - 1] = '\0'; // remove the newline
    } else {
        tokenBuffer[2] = "No hook description.";
    }
    return ERR_SUCCESS;
}

char* retrieveErrMsg(ERR_CODE ec) {
    switch (ec) {
        case ERR_SUCCESS: return "Success.";
        case ERR_FAILURE: return "Something failed, go debug it properly smh.";
        case ERR_NULL: return "NULL deteected. Graccceeeffulllyyyy exiting...";
        case ERR_PATH_TO_LONG: return "Provided path longer than 2048 characters! Seems a bit excessive?!";
        case ERR_INVALID_PATH: return "Invalid path format provided, please check it!";
        case ERR_INVALID_HOOKED_PATH: return "Hooked path is invalid, check update it with `mod` or manually via jumper.conf!";
        default: return "Default case, ERR msg not implemented.";
    }
}