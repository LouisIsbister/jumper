
#include "utils.h"

#ifndef COMMANDS_H
#define COMMANDS_H

errorc do_jump(const char *hook_name, FILE *conf_file);

errorc do_add(jmp_context_t *jctx, FILE *conf_file);
errorc do_mod(jmp_context_t* jctx, FILE *conf_file) ;

errorc do_del(jmp_context_t* jctx, FILE *conf_file);

errorc do_dir(jmp_context_t* jctx, FILE *conf_file);
errorc do_descr(jmp_context_t* jctx, FILE *conf_file);

errorc do_list(FILE *conf_file);
errorc do_help();


#endif // COMMANDS_H