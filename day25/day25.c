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

  char reader[50];
  while (true) {
    run(comp);
    ascii_out(comp);

    fgets(reader, 50, stdin);
    ascii_in(comp, reader);
  }

  free(work);
  free_comp(comp);
  return 0;
}