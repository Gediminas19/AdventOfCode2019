#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../intcode_comp/intcode_comp.h"

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
  lenA--;
  while (lenA > 0) {
    strncpy(routineA, to_process, lenA);
    // printf("Testing A of len %d: %s\n", lenA, routineA);
    replaceAll(workingA, 'A', routineA, to_process);

    int anchorB = strspn(workingA, "A,");
    int maxtermB = strcspn(workingA + anchorB, "A");
    int lenB = maxtermB < 21? maxtermB : 21;
    while (workingA[anchorB + lenB - 1] != ',') lenB--;
    lenB--;
    while (lenB > 0) {
      strncpy(routineB, workingA + anchorB, lenB);
      // printf("Testing B of len %d: %s\n", lenB, routineB);
      replaceAll(workingB, 'B', routineB, workingA);

      int anchorC = strspn(workingB, "AB,");
      int maxtermC = strcspn(workingB + anchorC, "AB");
      int lenC = maxtermC < 21? maxtermC : 21;
      while (workingB[anchorC + lenC - 1] != ',') lenC--;
      lenC--;
      while (lenC > 0) {
        strncpy(routineC, workingB + anchorC, lenC);
        // printf("Testing C of len %d: %s\n", lenC, routineC);
        replaceAll(workingC, 'C', routineC, workingB);
        // printf("%s\n", workingC);

        if (strspn(workingC, "ABC,") == strlen(workingC)) {
          strcpy(routineMain, workingC);
          return true;
        }

        memset(routineC, 0, 22*sizeof(char));
        lenC--;
        while (lenC > 0 && workingB[anchorC + lenC - 1] != ',') lenC--;
        lenC--;
      }

      memset(routineB, 0, 22*sizeof(char));
      lenB--;
      while (lenB > 0 && workingA[anchorB + lenB - 1] != ',') lenB--;
      lenB--;
    }

    memset(routineA, 0, 22*sizeof(char));
    lenA--;
    while (lenA > 0 && to_process[anchorA + lenA - 1] != ',') lenA--;
    lenA--;
  }

  return false;
}

int main() {
  int len;
  long *work = parse_input("input.txt", &len);

  intcode_comp *comp = init_comp(work, len);

  run(comp);

  int botx, boty, botdir;
  int row = 1, col = 1;
  char view[100][100]; // big enough
  memset(view, '.', 10000*sizeof(char));
  while (more_outputs(comp)) {
    char draw = (char) get_output(comp);
    printf("%c", draw);

    if (draw == '\n') {
      row++;
      col = 1;
    } else {
      if (draw != '.' && draw != '#') {
        botx = col;
        boty = row;
        switch (draw) {
          case '^':
            botdir = 0;
            break;
          case '>':
            botdir = 1;
            break;
          case 'v':
            botdir = 2;
            break;
          case '<':
            botdir = 3;
            break;
        }
      }
      view[row][col] = draw;
      col++;
    }
  }

  // Part 1
  int calabar = 0;
  for (int j = 2; j < 51; j++) {
    for (int k = 2; k < 45; k++) {
      if (view[j][k] == '#' && view[j - 1][k] == '#' && view[j + 1][k] == '#'
          && view[j][k - 1] == '#' && view[j][k + 1] == '#') calabar += (j-1)*(k-1);
    }
  }
  printf("Sum of alignment parameters: %d\n\n", calabar);

  // Part 2
  char *twists = calloc(250, sizeof(char));
  int cumdist = 0;
  while (true) {
    // printf("(%d, %d) - %d\n", botx, boty, botdir);
    int ax = botx, ay = boty, rx = botx, ry = boty, lx = botx, ly = boty;
    switch (botdir) {
      case 0:
        ay -= 1;
        lx -= 1;
        rx += 1;
        break;
      case 1:
        ax += 1;
        ly -= 1;
        ry += 1;
        break;
      case 2:
        ay += 1;
        lx += 1;
        rx -= 1;
        break;
      case 3:
        ax -= 1;
        ly += 1;
        ry -= 1;
        break;
    }

    if (view[ay][ax] == '#') {
      cumdist++;
      botx = ax;
      boty = ay;
    } else if (view[ay][ax] == '.') {
      if (cumdist > 0) sprintf(twists + strlen(twists), "%d,", cumdist);
      cumdist = 0;
      if (view[ly][lx] == '#') {
        botdir = (botdir - 1 + 4) % 4;
        strcat(twists, "L,");
      } else if (view[ry][rx] == '#') {
        botdir = (botdir + 1) % 4;
        strcat(twists, "R,");
      } else {
        twists[strlen(twists) - 1] = '\0';
        break;
      }
    }
  }
  printf("Total Moves Needed: %s\n", twists);

  char *routineA = calloc(22, sizeof(char));
  char *routineB = calloc(22, sizeof(char));
  char *routineC = calloc(22, sizeof(char));
  char *routineMain = calloc(22, sizeof(char));
  if (!decompose(twists, routineA, routineB, routineC, routineMain)) {
    printf("Failure to decompose path into 3 subroutines!");
    return 0;
  }

  work[0] = 2;
  reset_comp(comp, work, len);

  run(comp);
  ascii_out(comp);
  puts(routineMain);
  ascii_in_ln(comp, routineMain);

  run(comp);
  ascii_out(comp);
  puts(routineA);
  ascii_in_ln(comp, routineA);

  run(comp);
  ascii_out(comp);
  puts(routineB);
  ascii_in_ln(comp, routineB);

  run(comp);
  ascii_out(comp);
  puts(routineC);
  ascii_in_ln(comp, routineC);

  run(comp);
  ascii_out(comp);
  puts("n");
  ascii_in_ln(comp, "n");

  run(comp);
  ascii_out(comp);

  long dust = get_output(comp);
  printf("Dust collected: %ld\n", dust);

  free(work);
  free_comp(comp);
  return 0;
}