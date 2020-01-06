#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../intcode_comp/intcode_comp.h"

int main() {
  int len;
  long *work = parse_input("input.txt", &len);

  intcode_comp *comp = init_comp(work, len);

  // part 1
  int affected = 0;
  for (int y = 0; y < 50; y++) {
    for (int x = 0; x < 50; x++) {
      reset_comp(comp, work, len);
      add_input(comp, x);
      add_input(comp, y);
      run(comp);
      if (get_output(comp) == 1) affected++;
    }
  }
  printf("Affected locations: %d\n", affected);

  // part 2
  int ymin[1500], ymax[1500];

  int y0 = 4, x0 = 0;
  do {
    x0 += 1;
    reset_comp(comp, work, len);
    add_input(comp, x0);
    add_input(comp, y0);
    run(comp);
  } while (get_output(comp) != 1);
  ymin[y0] = x0;

  do {
    x0 += 1;
    reset_comp(comp, work, len);
    add_input(comp, x0);
    add_input(comp, y0);
    run(comp);
  } while (get_output(comp) != 0);
  ymax[y0] = x0 - 1;

  for (int y = 5; y < 1500; y++) {
    int x = ymin[y - 1] - 1;
    do {
      x += 1;
      reset_comp(comp, work, len);
      add_input(comp, x);
      add_input(comp, y);
      run(comp);
    } while (get_output(comp) != 1);
    ymin[y] = x;

    x = ymax[y - 1];
    do {
      x += 1;
      reset_comp(comp, work, len);
      add_input(comp, x);
      add_input(comp, y);
      run(comp);
    } while (get_output(comp) != 0);
    ymax[y] = x - 1;

    if (y > y0 + 99 && ymin[y] + 99 <= ymax[y - 99]) {
      printf("square found, upper left at %d,%d\n", ymin[y], y - 99);
      break;
    }
  }

  free(work);
  free_comp(comp);
  return 0;
}