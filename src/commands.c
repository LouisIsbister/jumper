
#include "commands.h"


/**
 * iterates through the entire file and prints each hook
 */
void list(FILE* confFile) {
    char hookEntry[MAX_HOOK_LENGTH];
    char* tokenBuffer[3];
    int hookNum = 1;

    printf("\nListing all hooks:\n\n");
    while (fgets(hookEntry, MAX_HOOK_LENGTH, confFile) != NULL) {
        // converts the token entry into name, hook dir, and description
        tokeniseHookEntry(tokenBuffer, hookEntry);

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