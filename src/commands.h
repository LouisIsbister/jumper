
#include "utils.h"

#ifndef COMMANDS_H
#define COMMANDS_H

void add(char *newHook, FILE *confFile);
void mod(uint16_t lineNumber, char *name, char *dir, char *descr, char *originalHook, FILE *confFile);
void del(uint16_t line, FILE *confFile);
void descr(char *hName, char *descr);
void list(FILE *confFile);
void help();

#endif // COMMANDS_H