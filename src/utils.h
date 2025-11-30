
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H


#if 1
#define CONF_FNAME                      "/home/louis/projects/jumper/jumper.conf.test"
#define TMP_CONF_FNAME                  "/home/louis/projects/jumper/jumper.conf.tmp.test"
#elif
#define CONF_FNAME                      "/etc/jumper.conf"
#define TMP_CONF_FNAME                  "/etc/jumper.conf.tmp"
#endif


#define MAX_ARG_COUNT                   3
#define MAX_HOOK_LENGTH                 1024
#define NUM_TOKENS_IN_HOOK              3



typedef struct hook_entry hook_entry_t;

typedef struct hook_entry {
        uint32_t line_number;
        char content[MAX_HOOK_LENGTH];
        char *tokens[NUM_TOKENS_IN_HOOK];
} hook_entry_t;


typedef enum jmp_flag {
        JUMP, 
        ADD,
        MOD,
        DEL,
        DIR,
        DESCR,
        LIST,
        HELP
} jmp_flag_t;


typedef enum error_code {
        ERR_SUCCESS,
        ERR_FAILURE,
        ERR_NULL,

        ERR_INVALID_CMD,

        ERR_PATH_TO_LONG,
        ERR_INVALID_PATH,
        ERR_UNKNOWN_HOOK,
        ERR_INVALID_HOOKED_PATH,
} errorc;

hook_entry_t *init_hook_entry(const char *hook_name, FILE *conf_file);
void cleanup_hook_entry(hook_entry_t *hook);

errorc populate_hook_entry(const char *hook_name, hook_entry_t* hook_entry, FILE *conf_file);

// errorc retrieve_hook(const char *target_hook_name, hook_entry_t *buffer, FILE *conf_file);
// errorc tokenise_hook(hook_entry_t *hook); 


void format_hook(char *hook_buffer, char *name, char *dir, char *descr);
void format_hook_from_tokens(char *hook_buffer, char **tokens);

void safe_file_close(FILE **file);

const char *retrieve_err_msg(errorc ec);



#define HELP_MSG "\n\n  ~ Welcome to jumper! ~\n\n"                                                                                                             \
        "The goal of this program is to be able to conviently `cd` to your frequently used directories.\n"                                                      \
        "This is achieved by creating a `hook` to said directories, then simply running the jumper program to go there.\n\n\n"                                  \
        "      ~ Commands: ~\n\n"                                                                                                                               \
                                                                                                                                                                \
        "`jumper <hook_name>`\n"                                                                                                                                \
        "  - *prints* the directory associated with the hook name to standard out\n\n"                                                                          \
                                                                                                                                                                \
        "`jumper -add <hook_name> -dir <dir> [-descr \"<description>\"]`\n"                                                                                     \
        "  - create a new hooked directory\n\n"                                                                                                                 \
                                                                                                                                                                \
        "`jumper -mod <hook_name> (-dir <new_dir> | -descr \"<new_description>\")`\n"                                                                           \
        "  - modify the contents of an existing hook. Both the directory and description are optional, but at least on must always be provided!\n\n"            \
                                                                                                                                                                \
        "`jumper -del <hook_name>`\n"                                                                                                                           \
        "  - delete a hook by its name\n\n"                                                                                                                     \
                                                                                                                                                                \
        "`jumper -dir <hook_name>`\n"                                                                                                                           \
        "  - print the directory associated with a hook\n\n"                                                                                                    \
                                                                                                                                                                \
        "`jumper -descr <hook_name>`\n"                                                                                                                         \
        "  - print the description associated with a hook\n\n"                                                                                                  \
                                                                                                                                                                \
        "`jumper -list`\n"                                                                                                                                      \
        "  - print all hooks alone with their descriptions\n\n"                                                                                                 \
                                                                                                                                                                \
        "`jumper -help`\n"                                                                                                                                      \
        "  - print all this information to the terminal\n\n\n\n"

#endif // UTILS_H