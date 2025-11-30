
#include "utils.h"
#include "jumper.h"

#include <string.h>
#include <assert.h>


hook_entry_t *
init_hook_entry() {
        hook_entry_t *hook = (hook_entry_t *) calloc(1, sizeof(hook_entry_t));
        assert(hook->line_number == 0);
        return hook;
}

void 
cleanup_hook_entry(hook_entry_t *hook) {
        for (uint8_t i = 0; i < NUM_TOKENS_IN_HOOK; i++) {
                if (hook->tokens[i] != NULL)
                        free(hook->tokens[i]);
        }
        free(hook);
}


/**
  *@brief find an copy the target hook into `buffer`
  *@return
 */
errorc 
retrieve_hook(const char *target_hook_name, hook_entry_t *hook, FILE *conf_file) {
        size_t t_hook_name_len = strlen(target_hook_name);
        
        uint32_t line = 1;
        char hook_entry[MAX_HOOK_LENGTH];
        while (fgets(hook_entry, MAX_HOOK_LENGTH, conf_file) != NULL) {
                if (strncmp(hook_entry, target_hook_name, t_hook_name_len) == 0) {
                        if (hook_entry[t_hook_name_len] != '|')
                                continue;

                        strncpy(hook->content, hook_entry, MAX_HOOK_LENGTH);
                        fseek(conf_file, 0, SEEK_SET);
                        return ERR_SUCCESS;
                }
                line++;
        }
        return ERR_UNKNOWN_HOOK;
}


errorc
tokenise_hook(hook_entry_t *hook) {
        char *save_state;
        char *token = strtok_r(hook->content, "|", &save_state);
        
        for (uint8_t i = 0; i < NUM_TOKENS_IN_HOOK; i++) {
                if (token == NULL) {
                        hook->tokens[i] = NULL;
                        continue;
                }

                hook->tokens[i] = (char *)calloc(strlen(token)+1, sizeof(char));
                if (hook->tokens[i] == NULL) {
                        printf("Failed to allocate memory for token: %s\n", token);
                        continue;
                }
                strncpy(hook->tokens[i], token, strlen(token));
                token = strtok_r(NULL, "|", &save_state);
        }

        if (token != NULL) {
                printf("Token left: '%s', hook context: '%s'\n", token, hook->content);
                return ERR_INVALID_HOOKED_PATH;
        }
        return ERR_SUCCESS;
}



void 
format_hook(char *buffer, char *name, char *dir, char *descr) {
        snprintf(buffer, MAX_HOOK_LENGTH, "%s|%s|%s\n", name, dir, descr);
}

void 
format_hook_from_tokens(char *buffer, char **tokens) {
        format_hook(buffer, tokens[0], tokens[1], tokens[2]);
}

void 
safe_file_close(FILE **file) {
        if (file != NULL && *file != NULL) {
                fclose(*file);
                *file = NULL;
        }
}

const char*
retrieve_err_msg(errorc ec) {
        switch (ec) {
                case ERR_SUCCESS: return "Success.";
                case ERR_FAILURE: return "Something failed, go debug it properly smh.";
                case ERR_NULL: return "NULL detected. Graccceeeffulllyyyy exiting...";
                case ERR_INVALID_CMD: return "Invalid command detected!";
                case ERR_PATH_TO_LONG: return "Provided path longer than 1024 characters! Seems a bit excessive?!";
                case ERR_INVALID_PATH: return "Invalid path format provided, please check it!";
                case ERR_UNKNOWN_HOOK: return "Unknown hook detected! Run with -list flag to view all hooks.";
                case ERR_INVALID_HOOKED_PATH: return "Hooked path has been corrupted, update it with `mod` or manually in jumper.conf!";
                default: return "[ERR] no msg for this error code.";
        }
}