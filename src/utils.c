
#include "utils.h"
#include <string.h>
#include <assert.h>

static bool strncmp_ext(const char* s1, const char* s2, uint8_t n);

static struct {
        const char* flag_name;
        jmp_flag_t flag;
} str_to_flag[] = {
        {"-add",   ADD},
        {"-mod",   MOD},
        {"-del",   DEL},
        {"-list",  LIST},
        {"-help",  HELP},
        {"-dir",   DIR},
        {"-descr", DESCR},
        {NULL, 0}
};

jmp_flag_t
flag_type_of(const char* str) {
        uint8_t l = sizeof(str_to_flag) / sizeof(str_to_flag[0]);
        for (uint8_t i = 0; i < l; i++) {
                const char* flg_name = str_to_flag[i].flag_name; 
                size_t arg_len = strlen(flg_name);
                if (strncmp_ext(flg_name, str, strlen(flg_name)))
                        return str_to_flag[i].flag;
        }
        return UNKNOWN;
}

static bool
strncmp_ext(const char* s1, const char* s2, uint8_t n) {
        assert(strlen(s1) == n);
        return strlen(s2) == n && strncmp(s1, s2, n) == 0;
}


const char* 
flag_type_to_str(const jmp_flag_t a) {
        uint8_t l = sizeof(str_to_flag) / sizeof(str_to_flag[0]);
        for (uint8_t i = 0; i < l; i++) {
                if (a == str_to_flag[i].flag)
                        return str_to_flag[i].flag_name;
        }
        return "[ERR] Unrecognised flag";
}


/**
 * @brief find an copy the target hook into `buffer`
 * @return the line number of this hook in the config file
 */
uint32_t 
retrieve_hook(char *target_hook_name, char *buffer, FILE *conf_file) {
        size_t t_hook_name_len = strlen(target_hook_name);
        
        uint32_t line = 1;
        char hook_entry[MAX_HOOK_LENGTH];
        while (fgets(hook_entry, MAX_HOOK_LENGTH, conf_file) != NULL) {
                if (strncmp(hook_entry, target_hook_name, t_hook_name_len) == 0) {
                        if (hook_entry[t_hook_name_len] != '|')
                                continue;

                        strncpy(buffer, hook_entry, MAX_HOOK_LENGTH);
                        fseek(conf_file, 0, SEEK_SET);
                        return line;
                }
                line++;
        }
        return 0;
}


errorc 
tokenise_hook(char **tokens, char *hook_entry) {
        char *save_state;
        char *token = strtok_r(hook_entry, "|", &save_state);
        
        uint8_t i;
        for (i = 0; i < NUM_FIELDS_IN_HOOK; i++) {
                tokens[i++] = token;
                token = strtok_r(NULL, "|", &save_state);
        }

        if (token != NULL)
                return ERR_INVALID_HOOKED_PATH;

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
                case ERR_UNKNOWN_HOOK: return "Unknown hook detected!";
                case ERR_INVALID_HOOKED_PATH: return "Hooked path has been corrupted, update it with `mod` or manually in jumper.conf!";
                default: return "[ERR] no msg for this error code.";
        }
}