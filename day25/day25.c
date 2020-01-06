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
  fclose(fp);

  intcode_comp *comp = init_comp(work, len);

  char reader[50];
  while (true) {
    run(comp);
    ascii_out(comp);

    fgets(reader, 50, stdin);
    ascii_in(comp, reader);
  }

  free(work);
  // free_comp(comp);
  return 0;
}