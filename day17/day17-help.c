#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "day17-help.h"

void replaceAll(char *working, char code, char *to_replace, char *origin) {
  int apos = -1;
  // working[apos] = code;
  char *searchstart = origin, *pch;
  while ((pch = strstr(searchstart, to_replace)) != NULL) {
    strncpy(working + apos + 1, searchstart, pch - searchstart);
    apos += 1 + pch - searchstart;
    working[apos] = code;
    searchstart = pch + strlen(to_replace);
  }
  strncpy(working + apos + 1, searchstart, origin + strlen(origin) - searchstart);
  working[apos + 1 + origin - searchstart + strlen(origin)] = '\0';
}

bool decompose(char *to_process, char *routineA, char *routineB, char *routineC, char *routineMain) {
  int total_len = strlen(to_process);
  char *workingA = calloc(total_len + 1, sizeof(char));
  char *workingB = calloc(total_len + 1, sizeof(char));
  char *workingC = calloc(total_len + 1, sizeof(char));

  int anchorA = 0;
  int lenA = 21;
  while (to_process[anchorA + lenA - 1] != ',') lenA--;
  while (lenA > 0) {
    strncpy(routineA, to_process, lenA);
    // printf("Testing A of len %d: %s\n", lenA, routineA);
    replaceAll(workingA, 'A', routineA, to_process);

    int anchorB = strspn(workingA, "A");
    int maxtermB = strcspn(workingA + anchorB, "A");
    int lenB = maxtermB < 21? maxtermB : 21;
    while (workingA[anchorB + lenB - 1] != ',') lenB--;
    while (lenB > 0) {
      strncpy(routineB, workingA + anchorB, lenB);
      // printf("Testing B of len %d: %s\n", lenB, routineB);
      replaceAll(workingB, 'B', routineB, workingA);

      int anchorC = strspn(workingB, "AB");
      int maxtermC = strcspn(workingB + anchorC, "AB");
      int lenC = maxtermC < 21? maxtermC : 21;
      while (workingB[anchorC + lenC - 1] != ',') lenC--;
      while (lenC > 0) {
        strncpy(routineC, workingB + anchorC, lenC);
        // printf("Testing C of len %d: %s\n", lenC, routineC);
        replaceAll(workingC, 'C', routineC, workingB);
        // printf("%s\n", workingC);

        if (strspn(workingC, "ABC") == strlen(workingC)) {
          routineA[strlen(routineA) - 1] = '\0';
          routineB[strlen(routineB) - 1] = '\0';
          routineC[strlen(routineC) - 1] = '\0';
          for (int j = 0; j < strlen(workingC); j++) {
            routineMain[j*2] = workingC[j];
            routineMain[j*2 + 1] = ',';
          }
          routineMain[strlen(routineMain) - 1] = '\0';
          return true;
        }

        memset(routineC, 0, 22*sizeof(char));
        lenC--;
        while (lenC > 0 && workingB[anchorC + lenC - 1] != ',') lenC--;
      }

      memset(routineB, 0, 22*sizeof(char));
      lenB--;
      while (lenB > 0 && workingA[anchorB + lenB - 1] != ',') lenB--;
    }

    memset(routineA, 0, 22*sizeof(char));
    lenA--;
    while (lenA > 0 && to_process[anchorA + lenA - 1] != ',') lenA--;
  }

  return false;
}