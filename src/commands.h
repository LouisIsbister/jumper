
#include "utils.h"

#ifndef COMMANDS_H
#define COMMANDS_H

errorc do_jump(const char *hook_name, FILE *conf_file);

void do_add(jmp_context_t *jctx, FILE *conf_file);

void do_list(FILE *conf_file);
void do_help();


void mod(uint16_t lineNumber, char *name, char *dir, char *descr, char *originalHook, FILE *confFile);
void del(uint16_t line, FILE *confFile);
void descr(char *hName, char *descr);

#endif // COMMANDS_H