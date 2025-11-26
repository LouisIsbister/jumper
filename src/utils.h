
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H


#define TESTING                         0
#define TEST_CONF_FNAME                 "jumper.conf.example"

#define MAX_ARG_COUNT                   3
#define CONF_FNAME                      "/etc/jumper.conf"
#define MAX_HOOK_LENGTH                 1024

#define NUM_FIELDS_IN_HOOK              3

typedef enum jmp_flag {
        // standalone commands. I.e. you could not include -add and -del in the same command 
        JUMP, 
        ADD,
        MOD,
        DEL,
        LIST,
        HELP,

        // additional flags used for setting the value of certain information
        DIR,
        DESCR,

        UNKNOWN
} jmp_flag_t;


typedef enum {
        ERR_SUCCESS,
        ERR_FAILURE,
        ERR_NULL,

        ERR_INVALID_CMD,

        ERR_PATH_TO_LONG,
        ERR_INVALID_PATH,
        ERR_UNKNOWN_HOOK,
        ERR_INVALID_HOOKED_PATH,
} errorc;


jmp_flag_t flag_type_of(const char* str);
const char* flag_type_to_str(const jmp_flag_t a);

uint32_t retrieve_hook(char *target_hook_name, char *buffer, FILE *conf_file);

errorc tokenise_hook(char **tokens, char *hook_entry);

void format_hook(char *hook_buffer, char *name, char *dir, char *descr);
void format_hook_from_tokens(char *hook_buffer, char **tokens);

void safe_file_close(FILE **file);

const char* retrieve_err_msg(errorc ec);


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