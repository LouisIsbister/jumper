
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

// set to 1 if in testing, otherise 0
#define TESTING 1

// #define MAX_ARG_COUNT                   3
#define CONF_FNAME                      "/etc/jumper.conf"
#define MAX_HOOK_LENGTH                 1024

typedef enum {
        ERR_SUCCESS,
        ERR_FAILURE,
        ERR_NULL,

        ERR_INVALID_CMD,

        ERR_PATH_TO_LONG,
        ERR_INVALID_PATH,
        ERR_UNKNOWN_HOOK,
        ERR_INVALID_HOOKED_PATH,
} ERR_CODE;


uint16_t retrieveHook(char *targetName, char *hookBuffer, FILE *confFile);

ERR_CODE tokeniseHookEntry(char **tokens, char *hookEntry);

void formatHook(char *hookBuffer, char *name, char *dir, char *descr);
void formatFromTokens(char *hookBuffer, char **tokens);

void safeFileClose(FILE **file);

char *retrieveErrMsg(ERR_CODE ec);


#define HELP_MSG "\n\n  ~ Welcome to jumper! ~\n\n"\
        "The goal of this program is to be able to conviently `cd` to your frequently used directories.\n"\
        "This is achieved by creating a `hook` to said directories, then simply running the jumper program to go there.\n\n\n"\
        "      ~ Commands: ~\n\n"\
        \
        "`jumper <hook_name>`\n"\
        "  - jump to the directory associated with the hook name\n\n"\
        \
        "`jumper -add <hook_name> -dir <dir> [-descr \"<description>\"]`\n"\
        "  - create a new hooked directory\n\n"\
        \
        "`jumper -mod <hook_name> [-dir <new_dir>] [-descr \"<new_description>\"]`\n"\
        "  - modify the contents of an existing hook. Both the directory and description are optional, providing neither will simply do nothing!\n\n"\
        \
        "`jumper -del <hook_name>`\n"\
        "  - delete a hook by its name\n\n"\
        \
        "`jumper -descr <hook_name>`\n"\
        "  - print the description associated with a hook\n\n"\
        \
        "`jumper -list`\n"\
        "  - print all hooks alone with their descriptions\n\n"\
        \
        "`jumper -help`\n"\
        "  - print all this information to the terminal\n\n\n\n"

#endif // UTILS_H