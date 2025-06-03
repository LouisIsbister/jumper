
#include "utils.h"

#ifndef COMMANDS_H
#define COMMANDS_H

void mod(HookEntry* hEntry, uint16_t line, FILE** confFile);
void del(uint16_t line, FILE** confFile);
void descr(HookEntry* hEntry, char* hookTokens);
void list(FILE* confFile);
void help();

#endif // COMMANDS_H