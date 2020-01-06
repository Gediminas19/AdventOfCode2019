#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../intcode_comp/intcode_comp.h"

int main() {
  int len;
  long *work = parse_input("input.txt", &len);

  intcode_comp *comp = init_comp(work, len);

  add_input(comp, 1);
  run(comp);
  printf("BOOST Keycode: %ld\n", get_output(comp));

  reset_comp(comp, work, len);
  add_input(comp, 2);
  run(comp);
  printf("Distress Signal Coordinates: %ld\n", get_output(comp));

  free(work);
  free_comp(comp);
  return 0;
}