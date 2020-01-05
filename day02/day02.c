#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run(int *work, int len, int a1, int a2);

int main()
{
  char read[500];
  scanf("%s\n", read);
  char *data = read;

  int len = 1;
  for (int j = 0; j < strlen(data); j++) if (data[j] ==',') len++;

  int *work = calloc(len, sizeof(int));
  int offset;
  int pos = 0;
  int num;
  while (sscanf(data, " %d,%n", &num, &offset) == 1)
  {
    data += offset;
    work[pos++] = num;
  }

  //Part 1
  printf("Part 1: %d\n", run(work, len, 12, 2));

  int desired = 19690720;

  for (int j = 0; j < 100; j++) {
    for (int k = 0; k < 100; k++) {
      if (run(work, len, j, k) == desired) {
        printf("Part 2: %d\n", 100*j + k);
        free(work);
        return 0;
      }
    }
  }

  puts("Part 2: -1");
  free(work);
  return 0;
}

int run(int *work_old, int len, int a1, int a2) {
  int *work = calloc(len, sizeof(int));
  memcpy(work, work_old, len*sizeof(int));
  work[1] = a1;
  work[2] = a2;

  int pc = 0;
  while (work[pc] != 99) {
    // printf("%d\n", work[pc]);
    switch (work[pc]) {
      case 1:
        work[work[pc + 3]] = work[work[pc + 1]] + work[work[pc + 2]];
        pc += 4;
        break;
      case 2:
        work[work[pc + 3]] = work[work[pc + 1]]*work[work[pc + 2]];
        pc += 4;
        break;
    }
  }
  int res = work[0];
  free(work);
  return res;
}