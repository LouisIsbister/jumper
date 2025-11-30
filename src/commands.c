
#include "jumper.h"

#include <string.h>
#include <assert.h>


static errorc write_changes(const char *content, uint32_t target_line_number, FILE* conf_file);
static jmp_arg_t* retrieve_arg_by_flag(jmp_context_t *jctx, jmp_flag_t type);


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
        
        printf(" Dscription of %s is: '%s'\n", descr_ctxt->value, hook->tokens[2]);
        cleanup_hook_entry(hook);
        return ERR_SUCCESS;
}



errorc
do_list(FILE *conf_file) {
        return ERR_SUCCESS;
}


errorc
do_help() {
        printf(HELP_MSG);
        return ERR_SUCCESS;
}







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
                printf("%d, %d\n", line, target_line_number);
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



// void 
// mod(uint16_t targetLine, char *hookName, char *hookDir, char *hookDescr, char *originalHook, FILE *confFile) {
//         // tokenise the hook entry into name, directory, and description
//         char *hTokens[3];
//         ERR_CODE ret = tokeniseHookEntry(hTokens, originalHook);

//         if (hookDir != NULL)   hTokens[1] = hookDir;
//         if (hookDescr != NULL) hTokens[2] = hookDescr;

//         char modifiedHook[MAX_HOOK_LENGTH];
//         formatFromTokens(modifiedHook, hTokens);

//         writeToTemporaryFile(targetLine, MOD, modifiedHook, confFile);
// }


// void 
// del(uint16_t targetLine, FILE *confFile) {
//         writeToTemporaryFile(targetLine, DEL, "", confFile);
// }


// void 
// writeToTemporaryFile(uint16_t targetLine, ACTION action, char *newHook, FILE *confFile) {
//         FILE *tmp = fopen("temp.tmp", "w");
//         if (tmp == NULL) {
//                 perror("File error");
//                 return;
//         }

//         uint16_t curLine = 1;
//         char hookEntry[MAX_HOOK_LENGTH];
//         while (fgets(hookEntry, MAX_HOOK_LENGTH, confFile) != NULL) {
//                 // if given MOD then append the new hook, otherwise skip the line for DEL
//                 if (curLine++ == targetLine) {
//                         if (action == MOD) {
//                                 fputs(newHook, tmp);
//                         }
//                         continue;
//                 }
//                 fputs(hookEntry, tmp);
//         }

//         fclose(tmp);
//         safeFileClose(&confFile);

// #if !TESTING
//         remove(CONF_FNAME);
//         rename("temp.tmp", CONF_FNAME);
// #endif
// }

// // // code to format conf file
// // // make sure every hook in the file has a \n on the end!
// // size_t hookLen = strlen(hookEntry);
// // if (hookEntry[hookLen - 1] != '\n') {
// //     char updated[hookLen + 1];
// //     snprintf(updated, hookLen + 1, "%s\n", hookEntry);
// //     fputs(updated, tmp);
// // } else {
// //     fputs(hookEntry, tmp);
// // }


// void
// descr(char *hookName, char *descr) {
//         printf("\nDescription for hook: '%s'\n", hookName);
//         printf(" ~ \"%s\"\n\n", descr);
// }


// /**
//   *iterates through the entire file and prints each hook
//  */
// void
// list(FILE *confFile) {
//         char hookEntry[MAX_HOOK_LENGTH];
//         int hookNum = 0;

//         ERR_CODE ret; printf("\n"); // just to get the seperator!
//         while (hookNum++, fgets(hookEntry, MAX_HOOK_LENGTH, confFile) != NULL) {
//                 // converts the token entry into name, hook dir, and description
//                 char *hTokens[3];
//                 ret = tokeniseHookEntry(hTokens, hookEntry);
//                 if (ret == ERR_INVALID_HOOKED_PATH) {
//                         printf("!! It appears the following hook may have been corrupted, please check .conf file:\nHook: %s\n\n", hookEntry);
//                         continue;
//                 }

//                 printf(" Hook No.%d:\n", hookNum);
//                 printf("  ~ Name: '%s'\n", hTokens[0]);
//                 printf("  ~ Path: '%s'\n", hTokens[1]);
//                 printf("  ~ Description: %s\n\n", hTokens[2]);
//         }

//         if (hookNum == 1) {
//                 printf("\n ~ Empty .config! No Hooks to List ~\n\n");
//         }
// }


// /**
//   *simply prints the help message
//  */
// void 
// help() {
//         printf(HELP_MSG);
// }   