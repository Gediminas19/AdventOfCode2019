#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../intcode_comp/intcode_comp.h"

int main() {
  int len;
  long *work = parse_input("input.txt", &len);

  intcode_comp *comp = init_comp(work, len);

  //Part 1
  add_input(comp, 1);
  run(comp);
  long test_result = 0;
  while (more_outputs(comp)) test_result = get_output(comp);
  printf("Input 1 Diagnostic Result: %ld\n", test_result);

  //Part 2
  reset_comp(comp, work, len);
  add_input(comp, 5);
  run(comp);
  printf("Input 5 Diagnostic Result: %ld\n", get_output(comp));

  free(work);
  free_comp(comp);
  return 0;
}