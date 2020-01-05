#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "intcode_comp.h"

int main() {
  FILE *fp = fopen("input.txt" , "r");

  int len;
  fscanf(fp, "%d\n", &len);

  long *work = calloc(len, sizeof(long));
  for (int j = 0; j < len; j++) fscanf(fp, "%li,", &work[j]);

  intcode_comp *comp = init_comp(work, len);

  run(comp);
  while (more_outputs(comp)) printf("%c", (char) get_output(comp));

  char run_commands[9][10] = {"NOT C J\n", "AND D J\n", "AND H J\n", "NOT A T\n", "OR T J\n", "NOT B T\n", "AND D T\n", "OR T J\n", "RUN\n"};
  for (int j = 0; j < 9; j++) for (int k = 0; k < strlen(run_commands[j]); k++)
    add_input(comp, run_commands[j][k]);

  run(comp);
  long out;
  while (more_outputs(comp)) {
    out = get_output(comp);
    printf("%c", (char) out);
  }
  printf("Running Hull Damage: %ld\n", out);


  reset_comp(comp, work, len);
  run(comp);
  while (more_outputs(comp)) printf("%c", (char) get_output(comp));

  char walk_commands[5][10] = {"NOT C J\n", "AND D J\n", "NOT A T\n", "OR T J\n", "WALK\n"};
  for (int j = 0; j < 5; j++) for (int k = 0; k < strlen(walk_commands[j]); k++)
    add_input(comp, walk_commands[j][k]);

  run(comp);
  while (more_outputs(comp)) {
    out = get_output(comp);
    printf("%c", (char) out);
  }
  printf("Walking Hull Damage: %ld\n", out);

  free(work);
  free_comp(comp);
  return 0;
}