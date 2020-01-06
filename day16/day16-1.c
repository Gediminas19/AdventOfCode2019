#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int apply_fft(int *work, int len) {
  int samp[4] = {0, 1, 0, -1};
  int **matrix = calloc(len, sizeof(int*));
  for (int j = 0; j < len; j++) {
    matrix[j] = calloc(len, sizeof(int));

    int mark = 0, k = -1;
    while (k < len) {
      for (int m = 0; m < j + 1; m++) {
        if (k >= 0) matrix[j][k] = samp[mark];
        if (++k >= len) break;
      }
      mark = (mark + 1) % 4;
    }
  }

  int *worktemp = calloc(len, sizeof(int));
  int iters = 100;
  for (int i = 0; i < iters; i++) {
    for (int j = 0; j < len; j++) {
      for (int k = 0; k < len; k++) {
        worktemp[j] += work[k]*matrix[j][k];
      }
      worktemp[j] = abs(worktemp[j]) % 10;
    }
    memcpy(work, worktemp, len*sizeof(int));
    memset(worktemp, 0, len*sizeof(int));
  }

  free(worktemp);
  for (int j = 0; j < len; j++) free(matrix[j]);
  free(matrix);
}

int main() {
  FILE *fp = fopen("input.txt" , "r");

  char read[1000];
  fscanf(fp, "%s", read);
  fclose(fp);

  int len = strlen(read);
  int *work = calloc(len, sizeof(int));
  for (int j = 0; j < len; j++) work[j] = read[j] - '0';

  // Part 1
  apply_fft(work, len);

  for (int j = 0; j < 8; j++) {
    printf("%d", work[j]);
  }
  puts("");

  free(work);

  return 0;
}