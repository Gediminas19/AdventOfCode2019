#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../intcode_comp/intcode_comp.h"

int main() {
  int len;
  long *work = parse_input("input.txt", &len);

  //Part 1
  work[1] = 12;
  work[2] = 2;
  intcode_comp *comp = init_comp(work, len);
  run(comp);
  printf("Value at position 0: %ld\n", comp->code[0]);

  int desired = 19690720;

  for (int j = 0; j < 100; j++) {
    for (int k = 0; k < 100; k++) {
      work[1] = j;
      work[2] = k;
      reset_comp(comp, work, len);
      run(comp);
      if (comp->code[0] == desired) {
        printf("Desired input: %d\n", 100*j + k);
        free(work);
        free_comp(comp);
        return 0;
      }
    }
  }

  puts("No input satisfies the desired result");
  free(work);
  free_comp(comp);
  return 0;
}