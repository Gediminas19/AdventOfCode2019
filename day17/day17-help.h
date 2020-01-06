#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef DAY17_HELP_H
#define DAY17_HELP_H

void replaceAll(char *working, char code, char *to_replace, char *origin);
bool decompose(char *to_process, char *routineA, char *routineB, char *routineC, char *routineMain);

#endif