
#include "jumper.h"
#include "commands.h"

void mod(HookEntry* hEntry, uint16_t targetLine, FILE** confFile) {
    FILE* tmp = fopen("temp.tmp", "w");
    if (tmp == NULL) {
        perror("File error");
        return;
    }

    char hookEntry[MAX_HOOK_LENGTH];
    uint16_t curLine = 1;
    while (fgets(hookEntry, MAX_HOOK_LENGTH, *confFile) != NULL) {
        if (curLine == targetLine) {
            // update teh hook entry, 'modifying' it
            memset(hookEntry, '\0', sizeof(hookEntry));
            sprintf(hookEntry, "%s|%s|%s\n", hEntry->hName, hEntry->hPath, hEntry->hDescr);
        }
        fputs(hookEntry, tmp);
        curLine++;
    }

    // closes and set FILE* to null to prevent double closing during cleanup
    safeFileClose(confFile);
    fclose(tmp);

    // remove(CONF_FNAME);
    // rename("temp.tmp", CONF_FNAME);
}

void del(uint16_t targetLine, FILE** confFile) {
    FILE* tmp = fopen("temp.tmp", "w");
    if (tmp == NULL) {
        perror("File error");
        return;
    }

    char hookEntry[MAX_HOOK_LENGTH];
    uint16_t curLine = 1;
    while (fgets(hookEntry, MAX_HOOK_LENGTH, *confFile) != NULL) {
        if (curLine == targetLine) { continue; } // effectively 'delete' the line 
        fputs(hookEntry, tmp);
        curLine++;
    }

    // closes and set FILE* to null to prevent double closing during cleanup
    safeFileClose(confFile);
    fclose(tmp);

    // remove(CONF_FNAME);
    // rename("temp.tmp", CONF_FNAME);
}

void descr(HookEntry* hEntry, char* descr) {
    printf("\nDescription for hook: '%s'\n", hEntry->hName);
    printf(" ~ \"%s\"\n\n", descr);
}

/**
 * iterates through the entire file and prints each hook
 */
void list(FILE* confFile) {
    char hookEntry[MAX_HOOK_LENGTH];
    char* tokenBuffer[3];
    int hookNum = 1;

    printf("\nListing all hooks:\n\n");
    ERR_CODE ret;
    while (fgets(hookEntry, MAX_HOOK_LENGTH, confFile) != NULL) {
        // converts the token entry into name, hook dir, and description
        ret = tokeniseHookEntry(tokenBuffer, hookEntry);
        if (ret == ERR_INVALID_HOOKED_PATH) {
            printf("!! It appears the following hook may have been corrupted, please check .conf file.");
        }

        printf(" ~ Hook No.%d:\n", hookNum);
        printf("Name: '%s'\n", tokenBuffer[0]);
        printf("Path: '%s'\n", tokenBuffer[1]);
        printf("Description: %s\n", tokenBuffer[2]);
        hookNum++;
    }
}

/**
 * simply prints the help message
 */
void help() {
    printf(HELP_MSG);
}