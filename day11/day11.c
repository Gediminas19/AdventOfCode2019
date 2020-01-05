#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "intcode_comp.h"

int main()
{
  int len;
  scanf("%d\n", &len);

  long *work = calloc(len, sizeof(long));
  for (int j = 0; j < len; j++) scanf("%li,", &work[j]);

  bool **painter = calloc(2001, sizeof(bool*));
  for (int j = 0; j < 2001; j++) painter[j] = calloc(2001, sizeof(bool));
  int **panelia = calloc(2001, sizeof(int*));
  for (int j = 0; j < 2001; j++) panelia[j] = calloc(2001, sizeof(int));
  int x = 1000, y = 1000, dir = 0;
  panelia[x][y] = 1; // Part 2; comment out for part 1

  intcode_comp *comp = init_comp(work, len);
  add_input(comp, panelia[x][y]);
  while (run(comp)) {
    int color = get_output(comp);
    int turn = get_output(comp)*2 - 1;
    printf("(%d, %d):: ", x, y);
    printf("Color: %d, Turn: %d\n", color, turn);
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
  printf("%d\n", painted);

  for (int j = 1500; j >= 1495; j--) {
    for (int k = 1500; k <= 1540; k++) {
      if (panelia[k][j] == 1) printf("\u25a0");
      else printf(" ");
    }
    puts("");
  }

  free(work);
  free(comp);
  return 0;
}