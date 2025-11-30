
#include "jumper.h"

#include <string.h>
#include <assert.h>


static errorc
write_changes(const char *content, uint32_t target_line_number, FILE* conf_file) {
        FILE *tmp = fopen(TMP_CONF_FNAME, "w");
        if (tmp == NULL) {
                printf(" [ERR] Failed to open temp file. Exiting...\n");
                return ERR_FAILURE;
        }

        uint16_t line = 1;
        char line_buffer[MAX_HOOK_LENGTH];
        while (fgets(line_buffer, MAX_HOOK_LENGTH, conf_file) != NULL) {
                if (line == target_line_number) {
                        fprintf(tmp, "%s", content);
                } else {
                        fprintf(tmp, "%s", line_buffer);
                }
                line++;
        }

        remove(CONF_FNAME);
        rename(TMP_CONF_FNAME, CONF_FNAME);

        return ERR_SUCCESS;
}


static jmp_arg_t*
retrieve_arg_by_flag(jmp_context_t *jctx, jmp_flag_t type) {
        uint8_t l = sizeof(jctx->args) / sizeof(jmp_arg_t*);
        for (uint8_t i = 0; i < l; i++) {
                jmp_arg_t *arg = jctx->args[i];
                if (arg == NULL)
                        continue;
                if (type == arg->flag->type)
                        return arg;
        }
        return NULL;
}



errorc
do_jump(const char *hook_name, FILE *conf_file) {
        errorc err;

        hook_entry_t *hook = init_hook_entry(hook_name, conf_file);
        if (hook == NULL)
                return ERR_FAILURE;
        
        assert(hook->tokens[1] != NULL);

        printf("%s", hook->tokens[1]);

        cleanup_hook_entry(hook);
        return ERR_SUCCESS;
}


errorc
do_add(jmp_context_t *jctx, FILE *conf_file) {
        char buffer[MAX_HOOK_LENGTH] = { 0 };

        jmp_arg_t *add_ctxt = retrieve_arg_by_flag(jctx, ADD);
        jmp_arg_t *dir_ctxt = retrieve_arg_by_flag(jctx, DIR);
        jmp_arg_t *descr_ctxt = retrieve_arg_by_flag(jctx, DESCR);

        int x = snprintf(buffer, MAX_HOOK_LENGTH, "%s|%s|", add_ctxt->value, dir_ctxt->value);
        if (descr_ctxt != NULL)
                snprintf(buffer+x, MAX_HOOK_LENGTH-x, "%s\n", descr_ctxt->value);
        else
                snprintf(buffer+x, MAX_HOOK_LENGTH-x, "[empty]\n");

        fwrite(buffer, sizeof(char), strlen(buffer), conf_file);
        fflush(conf_file);
        
        return ERR_SUCCESS;
}


errorc
do_mod(jmp_context_t* jctx, FILE *conf_file) {
        errorc err;

        jmp_arg_t *mod_ctxt = retrieve_arg_by_flag(jctx, MOD);
        jmp_arg_t *dir_ctxt = retrieve_arg_by_flag(jctx, DIR);
        jmp_arg_t *descr_ctxt = retrieve_arg_by_flag(jctx, DESCR);

        hook_entry_t *hook = init_hook_entry(mod_ctxt->value, conf_file);
        if (hook == NULL)
                return ERR_FAILURE;
       
        char buffer[MAX_HOOK_LENGTH] = { 0 };
        snprintf(buffer, MAX_HOOK_LENGTH, 
                "%s|%s|%s\n", 
                mod_ctxt->value,
                dir_ctxt == NULL ? hook->tokens[1] : dir_ctxt->value,
                descr_ctxt == NULL ? hook->tokens[2] : descr_ctxt->value);

        err = write_changes(buffer, hook->line_number, conf_file);
        if (err != ERR_SUCCESS) {
                cleanup_hook_entry(hook);
                return err;
        }

        cleanup_hook_entry(hook);
        return ERR_SUCCESS;
}



errorc
do_del(jmp_context_t* jctx, FILE *conf_file) {
        errorc err;

        jmp_arg_t *del_ctxt = retrieve_arg_by_flag(jctx, DEL);

        hook_entry_t *hook = init_hook_entry(del_ctxt->value, conf_file);
        if (hook == NULL)
                return ERR_FAILURE;

        err = write_changes("", hook->line_number, conf_file);
        if (err != ERR_SUCCESS) {
                cleanup_hook_entry(hook);
                return err;
        }

        cleanup_hook_entry(hook);
        return ERR_SUCCESS;
}


errorc 
do_dir(jmp_context_t* jctx, FILE *conf_file) {
        jmp_arg_t *dir_ctxt = retrieve_arg_by_flag(jctx, DIR);
        hook_entry_t *hook = init_hook_entry(dir_ctxt->value, conf_file);
        if (hook == NULL)
                return ERR_FAILURE;
        
        printf(" %s is hooked to directory: '%s'\n", dir_ctxt->value, hook->tokens[1]);
        cleanup_hook_entry(hook);
        return ERR_SUCCESS;
}


errorc 
do_descr(jmp_context_t* jctx, FILE *conf_file) {
        jmp_arg_t *descr_ctxt = retrieve_arg_by_flag(jctx, DESCR);
        hook_entry_t *hook = init_hook_entry(descr_ctxt->value, conf_file);
        if (hook == NULL)
                return ERR_FAILURE;
        
        printf(" The description of %s is: '%s'\n", descr_ctxt->value, hook->tokens[2]);
        cleanup_hook_entry(hook);
        return ERR_SUCCESS;
}



errorc
do_list(FILE *conf_file) {
        hook_entry_t *hook_entry = (hook_entry_t *) calloc(1, sizeof(hook_entry_t));
        if (hook_entry == NULL) {
                printf(" [ERR] Failed to allocate hook_entry_t. Exiting...\n");
                return ERR_FAILURE;
        }

        char buffer[MAX_HOOK_LENGTH] = { 0 };
        uint32_t line = 1;
        while (fgets(buffer, MAX_HOOK_LENGTH, conf_file)) {
                strncpy(hook_entry->content, buffer, MAX_HOOK_LENGTH);
                tokenise_hook(hook_entry);

                printf("\n Hook No.%d:\n", line++);
                printf("  ~ Name: '%s'\n", hook_entry->tokens[0]);
                printf("  ~ Path: '%s'\n", hook_entry->tokens[1]);
                printf("  ~ Description: %s\n", hook_entry->tokens[2]);
        }

        if (line == 1)
                printf("\n ~ Empty .config! No Hooks to List ~\n\n");

        return ERR_SUCCESS;
}

errorc
do_help() {
        printf(HELP_MSG);
        return ERR_SUCCESS;
}



