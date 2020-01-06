#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../intcode_comp/intcode_comp.h"

int main() {
  int len;
  long *work = parse_input("input.txt", &len);
  work[0] = 2;

  int board[23][45];
  int score = 0, blocks = 0;
  int ballx, bally, padx, pady;

  intcode_comp *comp = init_comp(work, len);

  //Part 1
  run(comp);
  while (more_outputs(comp)) {
    int x = get_output(comp);
    int y = get_output(comp);
    int t = get_output(comp);

    if (x == -1 && y == 0) {
      score = t;
      continue;
    }

    switch (t) {
      case 0:
        board[y][x] = ' ';
        break;
      case 1:
        board[y][x] = '#';
        break;
      case 2:
        board[y][x] = 'X';
        break;
      case 3:
        board[y][x] = '-';
        break;
      case 4:
        board[y][x] = '@';
        break;
    }

    if (t == 2) blocks++;
  }
  // Print starting board
  for (int y = 0; y < 23; y++) {
    for (int x = 0; x < 45; x++) {
      printf("\033[%d;%dH", y + 2, x + 1);
      printf("%c", board[y][x]);
    }
  }
  printf("\033[%d;%dH", 26, 0);
  printf("Blocks: %d | Score: %d", blocks, score);
  getchar();

  // Part 2
  while (blocks != 0) {
    // simple paddle AI
    if (ballx == padx) add_input(comp, 0);
    else if (ballx < padx) add_input(comp, -1);
    else add_input(comp, 1);

    run(comp);

    while (more_outputs(comp)) {
      int x = get_output(comp);
      int y = get_output(comp);
      int t = get_output(comp);

      if (x == -1 && y == 0) {
        score = t;
        continue;
      }

      if (board[y][x] == 'X' && t != 'X') blocks--;

      switch (t) {
        case 0:
          board[y][x] = ' ';
          break;
        case 1:
          board[y][x] = '#';
          break;
        case 2:
          board[y][x] = 'X';
          break;
        case 3:
          board[y][x] = '-';
          padx = x;
          pady = y;
          break;
        case 4:
          board[y][x] = '@';
          ballx = x;
          bally = y;
          break;
      }
      printf("\033[%d;%dH", y + 2, x + 1);
      printf("%c", board[y][x]);
    }
    printf("\033[%d;%dH", 26, 0);
    printf("Blocks: %3d | Score: %5d", blocks, score);
    usleep(1500);
  }
  puts("");

  free(work);
  free(comp);
  return 0;
}