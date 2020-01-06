#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "intcode_comp.h"
#include "day17-help.h"

int main() {
  FILE *fp = fopen("input.txt" , "r");

  int len;
  fscanf(fp, "%d\n", &len);

  long *work = calloc(len, sizeof(long));
  for (int j = 0; j < len; j++) fscanf(fp, "%li,", &work[j]);
  fclose(fp);

  intcode_comp *comp = init_comp(work, len);

  run(comp);

  int botx, boty, botdir;
  int row = 1, col = 1;
  char view[53][47]; // found by reading output manually
  memset(view, '.', 53*47*sizeof(char));
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
        sprintf(twists + strlen(twists), "L,");
      } else if (view[ry][rx] == '#') {
        botdir = (botdir + 1) % 4;
        sprintf(twists + strlen(twists), "R,");
      } else break;
    }
  }
  printf("Total Moves Needed: %s\n", twists);

  char *routineA = calloc(22, sizeof(char));
  char *routineB = calloc(22, sizeof(char));
  char *routineC = calloc(22, sizeof(char));
  char *routineMain = calloc(22, sizeof(char));
  decompose(twists, routineA, routineB, routineC, routineMain);

  work[0] = 2;
  reset_comp(comp, work, len);

  run(comp);
  ascii_out(comp);
  printf("%s", routineMain);
  ascii_in(comp, routineMain);

  run(comp);
  ascii_out(comp);
  printf("%s", routineA);
  ascii_in(comp, routineA);

  run(comp);
  ascii_out(comp);
  printf("%s", routineB);
  ascii_in(comp, routineB);

  run(comp);
  ascii_out(comp);
  printf("%s", routineC);
  ascii_in(comp, routineC);

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