#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../intcode_comp/intcode_comp.h"

bool recurse(intcode_comp *comp, char **room, int x, int y, int attempt,
             int dist, bool halt, int *solution, int *maxdist) {
  // printf("attempting %d %d\n", x, y);
  switch (attempt) {
    case 1:
      y -= 1;
      break;
    case 2:
      y += 1;
      break;
    case 3:
      x -= 1;
      break;
    case 4:
      x += 1;
      break;
  }

  // if visited before, don't even attempt
  if (room[x][y] != 'X') return false;

  // otherwise, attempt to visit
  add_input(comp, attempt);
  run(comp);
  long status = get_output(comp);

  // oxygen system found!
  if (halt && status == 2) {
    room[x][y] = 'o';
    // printf("found at %d %d\n", x, y);
    // printf("Travled %d tiles\n", dist);
    solution[dist] = attempt;
    solution[0] = dist;
    return true;
  }

  // if blocked, mark it as such
  if (status == 0) {
    room[x][y] = '#';
    // printf("blocked at %d %d\n", x, y);
    return false;
  }

  // for part 2, record maxdist of an empty space
  if (!halt && dist > *maxdist) *maxdist = dist;

  // status 1 means the robot has reached another empty cell, now recurse
  room[x][y] = ' ';
  for (int j = 1; j <= 4; j++) {
    if (recurse(comp, room, x, y, j, dist + 1, halt, solution, maxdist)) {
      solution[dist] = attempt;
      return true;
    }
  }

  // if here, then all further recursing led to dead ends, so backtrack!
  int backmand = -1;
  switch (attempt) {
    case 1:
      backmand = 2;
      break;
    case 2:
      backmand = 1;
      break;
    case 3:
      backmand = 4;
      break;
    case 4:
      backmand = 3;
      break;
  }
  add_input(comp, backmand);
  run(comp);
  // this is just to flush, since we know we backtracked into empty cell
  get_output(comp);

  return false;
}

bool search(intcode_comp *comp, char **room, int x, int y, int *solution) {
  // begin search by marking current spot as empty and recursing
  room[x][y] = ' ';
  for (int j = 1; j <= 4; j++) {
    if (recurse(comp, room, x, y, j, 1, true, solution, NULL)) return true;
  }
  return false;
}

void sweep(intcode_comp *comp, char **room, int x, int y, int *maxdist) {
  // begin search by marking current spot as empty and recursing
  room[x][y] = 'o';
  for (int j = 1; j <= 4; j++) {
    recurse(comp, room, x, y, j, 1, false, NULL, maxdist);
  }
}

int main() {
  int len;
  long *work = parse_input("input.txt", &len);

  char **room = calloc(2001, sizeof(char*));
  for (int j = 0; j < 2001; j++) {
    room[j] = calloc(2001, sizeof(char));
    memset(room[j], 'X', sizeof(char)*2001);
  }
  int x = 1000, y = 1000, dir = 0;

  intcode_comp *comp = init_comp(work, len);

  //Part 1
  int solution[1000];
  search(comp, room, x, y, solution);
  printf("Dist to oxygen: %d\n", solution[0]);

  // print partial map
  for (int j = 978; j < 1022; j++) {
    for (int k = 977; k < 1023; k++) {
      printf("%c", room[k][j]);
    }
    puts("");
  }
  puts("");

  // Part 2
  // start by positioning droid to oxygen system
  for (int j = 1; j <= solution[0]; j++) {
    switch (solution[j]) {
      case 1:
        y -= 1;
        break;
      case 2:
        y += 1;
        break;
      case 3:
        x -= 1;
        break;
      case 4:
        x += 1;
        break;
    }
    add_input(comp, solution[j]);
    run(comp);
    get_output(comp);
  }

  // reset map info
  for (int j = 0; j < 2001; j++) {
    memset(room[j], 'X', sizeof(char)*2001);
  }

  // do a full sweep of the room
  int *maxdist = calloc(1, sizeof(int));
  sweep(comp, room, x, y, maxdist);
  printf("Maxdist from oxygen: %d\n", *maxdist);

  // print full map
  for (int j = 978; j < 1022; j++) {
    for (int k = 977; k < 1023; k++) {
      printf("%c", room[k][j]);
    }
    puts("");
  }

  free(work);
  free(comp);
  return 0;
}