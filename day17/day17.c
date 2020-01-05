#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "intcode_comp.h"

int main() {
  int len;
  scanf("%d\n", &len);

  long *work = calloc(len, sizeof(long));
  for (int j = 0; j < len; j++) scanf("%li,", &work[j]);

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
  // printf("Rows: %d, Cols: %d\n", rows, cols);

  // Part 1
  int calabar = 0;
  for (int j = 2; j < 51; j++) {
    for (int k = 2; k < 45; k++) {
      if (view[j][k] == '#' && view[j - 1][k] == '#' && view[j + 1][k] == '#'
          && view[j][k - 1] == '#' && view[j][k + 1] == '#') calabar += (j-1)*(k-1);
    }
  }
  printf("Sum of alignment parameters: %d\n", calabar);

  for (int j = 0; j < 53; j++) {
    for (int k = 0; k < 47; k++) {
      printf("%c", view[j][k]);
    }
    puts("");
  }
  free(comp);

  // Part 2
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
      if (cumdist > 0) printf("%d,", cumdist);
      cumdist = 0;
      if (view[ly][lx] == '#') {
        botdir = (botdir - 1 + 4) % 4;
        printf("L,");
      } else if (view[ry][rx] == '#') {
        botdir = (botdir + 1) % 4;
        printf("R,");
      } else {
        puts("");
        break;
      }
    }
  }

  char comb[] = "A,B,A,B,C,C,B,C,B,A\n";
  char funca[] = "R,12,L,8,R,12\n";
  char funcb[] = "R,8,R,6,R,6,R,8\n";
  char funcc[] = "R,8,L,8,R,8,R,4,R,4\n";
  char feed[] = "n\n";

  work[0] = 2;
  comp = init_comp(work, len);

  run(comp);
  while (more_outputs(comp)) printf("%c", (char) get_output(comp));
  for (int j = 0; j < strlen(comb); j++) { printf("%c", comb[j]); add_input(comp, comb[j]); }
  run(comp);
  while (more_outputs(comp)) printf("%c", (char) get_output(comp));
  for (int j = 0; j < strlen(funca); j++) { printf("%c", funca[j]); add_input(comp, funca[j]); }
  run(comp);
  while (more_outputs(comp)) printf("%c", (char) get_output(comp));
  for (int j = 0; j < strlen(funcb); j++) { printf("%c", funcb[j]); add_input(comp, funcb[j]); }
  run(comp);
  while (more_outputs(comp)) printf("%c", (char) get_output(comp));
  for (int j = 0; j < strlen(funcc); j++) { printf("%c", funcc[j]); add_input(comp, funcc[j]); }
  run(comp);
  while (more_outputs(comp)) printf("%c", (char) get_output(comp));
  for (int j = 0; j < strlen(feed); j++) { printf("%c", feed[j]); add_input(comp, feed[j]); }
  run(comp);

  long dust;
  while (more_outputs(comp)) {
    dust = get_output(comp);
    printf("%c", (char) dust);
  }
  printf("Dust collected: %ld\n", dust);


  free(work);
  free(comp);
  return 0;
}