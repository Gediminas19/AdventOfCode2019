#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../intcode_comp/intcode_comp.h"

int paint(intcode_comp *comp, bool start_on_white) {
  bool **painter = calloc(2001, sizeof(bool*));
  for (int j = 0; j < 2001; j++) painter[j] = calloc(2001, sizeof(bool));
  int **panelia = calloc(2001, sizeof(int*));
  for (int j = 0; j < 2001; j++) panelia[j] = calloc(2001, sizeof(int));
  int x = 1000, y = 1000, dir = 0;
  if (start_on_white) panelia[x][y] = 1;

  add_input(comp, panelia[x][y]);
  while (run(comp)) {
    int color = get_output(comp);
    int turn = get_output(comp)*2 - 1;
    // printf("(%d, %d):: ", x, y);
    // printf("Color: %d, Turn: %d\n", color, turn);
    panelia[x][y] = color;
    painter[x][y] = true;
    dir = (dir + turn + 4) % 4;
    switch (dir) {
      case 0:
        y += 1;
        break;
      case 1:
        x += 1;
        break;
      case 2:
        y -= 1;
        break;
      case 3:
        x -= 1;
        break;
    }
    add_input(comp, panelia[x][y]);
  }

  int painted = 0;
  for (int j = 0; j < 2001; j++) {
    for (int k = 0; k < 2001; k++) {
      if (painter[j][k]) painted++;
    }
  }

  if (!start_on_white) return painted;

  for (int j = 1000; j >= 995; j--) {
    for (int k = 1000; k <= 1040; k++) {
      if (panelia[k][j] == 1) printf("\u25a0");
      else printf(" ");
    }
    puts("");
  }

  free_comp(comp);
  return painted;
}

int main() {
  int len;
  long *work = parse_input("input.txt", &len);

  intcode_comp *comp = init_comp(work, len);
  printf("Part 1 Total Painted Panels: %d\n", paint(comp, false));

  reset_comp(comp, work, len);
  puts("Part 2 Code:");
  paint(comp, true);

  free(work);
  return 0;
}