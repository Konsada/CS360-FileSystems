#ifndef LAB3_H
#define LAB3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sh.h"

void mysh(int argc, char *argv[], char *env[]);
void tokenLine(char *str, char *tokens[]);
int strWordCount(char *str);
void strTokenizer(char *str);
void mystrcpy(char destination[], char *source);

#endif
