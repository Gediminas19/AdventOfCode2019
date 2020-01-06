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

  run(comp);
  ascii_out(comp);

  char *walk_commands[] = {"NOT C J", "AND D J", "NOT A T", "OR T J", "WALK"};
  for (int j = 0; j < 5; j++) {
    puts(walk_commands[j]);
    ascii_in_ln(comp, walk_commands[j]);
  }
  run(comp);
  ascii_out(comp);
  printf("Walking Hull Damage: %ld\n\n", get_output(comp));

  reset_comp(comp, work, len);
  run(comp);
  ascii_out(comp);

  char *run_commands[] = {"NOT C J", "AND D J", "AND H J", "NOT A T", "OR T J", "NOT B T", "AND D T", "OR T J", "RUN"};
  for (int j = 0; j < 9; j++) {
    puts(run_commands[j]);
    ascii_in_ln(comp, run_commands[j]);
  }
  run(comp);
  ascii_out(comp);
  printf("Running Hull Damage: %ld\n", get_output(comp));

  free(work);
  free_comp(comp);
  return 0;
}