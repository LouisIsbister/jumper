
#include "jumper.h"
#include "commands.h"


void writeToTemporaryFile(uint16_t targetLine, ACTION action, char *newHook, FILE *confFile);


void add(char *newHook, FILE *confFile) {
    fputs(newHook, confFile);
}


void mod(uint16_t targetLine, char *hookName, char *hookDir, char *hookDescr, char *originalHook, FILE *confFile) {
    // tokenise the hook entry into name, directory, and description
    char *hTokens[3];
    ERR_CODE ret = tokeniseHookEntry(hTokens, originalHook);

    if (hookDir != NULL)   hTokens[1] = hookDir;
    if (hookDescr != NULL) hTokens[2] = hookDescr;

    char modifiedHook[MAX_HOOK_LENGTH];
    formatFromTokens(modifiedHook, hTokens);

    writeToTemporaryFile(targetLine, MOD, modifiedHook, confFile);
}


void del(uint16_t targetLine, FILE *confFile) {
    writeToTemporaryFile(targetLine, DEL, "", confFile);
}


void writeToTemporaryFile(uint16_t targetLine, ACTION action, char *newHook, FILE *confFile) {
    FILE *tmp = fopen("temp.tmp", "w");
    if (tmp == NULL) {
        perror("File error");
        return;
    }

    uint16_t curLine = 1;
    char hookEntry[MAX_HOOK_LENGTH];
    while (fgets(hookEntry, MAX_HOOK_LENGTH, confFile) != NULL) {
        // if given MOD then append the new hook, otherwise skip the line for DEL
        if (curLine++ == targetLine) {
            if (action == MOD) {
                fputs(newHook, tmp);
            }
            continue;
        }
        fputs(hookEntry, tmp);
    }

    fclose(tmp);
    safeFileClose(&confFile);

#if !TESTING
    remove(CONF_FNAME);
    rename("temp.tmp", CONF_FNAME);
#endif
}

// // code to format conf file
// // make sure every hook in the file has a \n on the end!
// size_t hookLen = strlen(hookEntry);
// if (hookEntry[hookLen - 1] != '\n') {
//     char updated[hookLen + 1];
//     snprintf(updated, hookLen + 1, "%s\n", hookEntry);
//     fputs(updated, tmp);
// } else {
//     fputs(hookEntry, tmp);
// }


void descr(char *hookName, char *descr) {
    printf("\nDescription for hook: '%s'\n", hookName);
    printf(" ~ \"%s\"\n\n", descr);
}


/**
 * iterates through the entire file and prints each hook
 */
void list(FILE *confFile) {
    char hookEntry[MAX_HOOK_LENGTH];
    int hookNum = 0;

    ERR_CODE ret; printf("\n"); // just to get the seperator!
    while (hookNum++, fgets(hookEntry, MAX_HOOK_LENGTH, confFile) != NULL) {
        // converts the token entry into name, hook dir, and description
        char *hTokens[3];
        ret = tokeniseHookEntry(hTokens, hookEntry);
        if (ret == ERR_INVALID_HOOKED_PATH) {
            printf("!! It appears the following hook may have been corrupted, please check .conf file:\nHook: %s\n\n", hookEntry);
            continue;
        }

        printf(" Hook No.%d:\n", hookNum);
        printf("  ~ Name: '%s'\n", hTokens[0]);
        printf("  ~ Path: '%s'\n", hTokens[1]);
        printf("  ~ Description: %s\n\n", hTokens[2]);
    }

    if (hookNum == 1) {
        printf("\n ~ Empty .config! No Hooks to List ~\n\n");
    }
}


/**
 * simply prints the help message
 */
void help() {
    printf(HELP_MSG);
}