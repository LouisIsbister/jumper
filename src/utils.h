
#include <windows.h>
#include <stdio.h>

#ifndef UTILS_H
#define UTILS_H

#define CONF_FNAME "\\jumper.conf"
#define CONF_FNAME_LEN 13  // length of conf fname + 1 for the null terminator
#define MAX_HOOK_LENGTH 1024

#define ADD "-add"
#define MOD "-mod"
#define DESCR "-descr"
#define DIR "-dir"
#define DEL "-del"
#define LIST "-list"
#define HELP "-help"

typedef enum {
    ERR_SUCCESS,
    ERR_FAILURE,
    ERR_NULL,

    ERR_PATH_TO_LONG,
    ERR_INVALID_PATH,
    ERR_INVALID_HOOKED_PATH,
} ERR_CODE;

ERR_CODE retrieveConfFile(char** path);

ERR_CODE tokeniseHookEntry(char** tokenBuffer, char* hookEntry);

char* retrieveErrMsg(ERR_CODE ec);


#define HELP_MSG "\n\n ~ Welcome to jumper! ~\n\n"\
    "The goal of this program is to be able to conviently `cd` to your frequenctly used directoies.\n"\
    "This is achieved by creating a `hook` to said directories, then simply running th jumper program to go there.\n\n"\
    "Commads:\n"\
    "`jumper <hook_name>`\n"\
    "  - jump to the directory associated with the hook name\n\n"\
    "`jumper -add <hook_name> -dir <dir> [-descr \" ... description ... \"]`\n"\
    "  - add a new hooked directory\n\n"\
    "`jumper -mod <hook_name> -dir <new_dir> [-descr \" ... description ... \"]`\n"\
    "  - modeifiy an existing hookname\n\n"\
    "`jumper -descr <hook_name>`\n"\
    "  - print the description associated with a hook\n\n"\
    "`jumper -del <hook_name>`\n"\
    "  - delete a hook\n\n"\
    "`jumper -list`\n"\
    "  - print all hooks alone with their descriptions\n\n"\
    "`jumper -help`\n"\
    "  - print this information again\n\n"

#endif // UTILS_H