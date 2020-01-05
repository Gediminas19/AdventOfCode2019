#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int run(int *work, int len, int a1);

int main()
{
  int len;
  scanf("%d\n", &len);

  int *work = calloc(len, sizeof(int));
  for (int j = 0; j < len; j++) {
    scanf("%d,", &work[j]);
  }

  // for (int j = 0; j < len; j++) {
  //   printf("%d ", work[j]);
  // }

  //Part 1
  run(work, len, 1);

  //Part 2
  run(work, len, 5);

  free(work);
  return 0;
}

int run(int *work_old, int len, int a1) {
  int *work = calloc(len, sizeof(int));
  memcpy(work, work_old, len*sizeof(int));

  int pc = 0, opcode = -1;
  int params[3];
  while (work[pc] != 99) {
    // printf("pc %d: %d\n", pc, work[pc]);
    opcode = work[pc] % 100;
    int temp = work[pc]/100;
    for (int i = 0; i < 3; i++) {
      if (temp % 10 == 0 && work[pc + 1 + i] < len) params[i] = work[work[pc + 1 + i]];
      else params[i] = work[pc + 1 + i];
      temp /= 10;
      // printf("param %d: %d\n", i, params[i]);
    }

    switch (opcode) {
      case 1:
        work[work[pc + 3]] = params[0] + params[1];
        pc += 4;
        break;
      case 2:
        work[work[pc + 3]] = params[0]*params[1];
        pc += 4;
        break;
      case 3:
        work[work[pc + 1]] = a1;
        pc += 2;
        break;
      case 4:
        printf("%d\n", params[0]);
        pc += 2;
        break;
      case 5:
        if (params[0] != 0) pc = params[1];
        else pc += 3;
        break;
      case 6:
        if (params[0] == 0) pc = params[1];
        else pc += 3;
        break;
      case 7:
        work[work[pc + 3]] = params[0] < params[1]? 1 : 0;
        pc += 4;
        break;
      case 8:
        work[work[pc + 3]] = params[0] == params[1]? 1 : 0;
        pc += 4;
        break;
    }
  }
  int res = work[0];
  free(work);
  return res;
}