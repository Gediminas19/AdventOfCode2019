#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *fp = fopen("input.txt" , "r");

  char read[1000];
  fscanf(fp, "%s", read);
  fclose(fp);

  int len = strlen(read);
  int *work = calloc(len, sizeof(int));
  for (int j = 0; j < len; j++) work[j] = read[j] - '0';

  read[7] = '\0';
  int offset = atoi(read);
  int grand_len = len*10000;
  printf("Offset: %d, Total Len: %d\n", offset, grand_len);
  int necessary = grand_len - offset;

  // If we consider the nxn FFT matrix, we quickly realize that after the (n/2)^th row
  // the matrix consists of 0s until the diagonal then 1s on the diagonal and after
  // this means the bottom half of each output vector is actually just a partial sum
  // where the (n - k)^th entry of the output vector is the sum of the last k
  // entries of the input vector mod 10

  // since the offset is well in the 2nd half of the vector, we can work with only
  // the entries beginning at the offset until the end of the vector; entries higher
  // up have no impact on the entries further down, while entries further down impact
  // all entries further up

  // in fact, each phase we simply calculate the cumulative sum from the end of the
  // vector while taking mod 10; this cumulative sum vector is then the input vector
  // for the next phase!
  int *worksmall = calloc(necessary + 1, sizeof(int));
  for (int j = offset; j < grand_len; j++) worksmall[j - offset] = work[j%len];
  for (int i = 0; i < 100; i++)
    for (int j = necessary - 1; j >= 0; j--) worksmall[j] = (worksmall[j + 1] + worksmall[j])%10;

  for (int j = 0; j < 8; j++) printf("%d", worksmall[j]);
  puts("");

  free(work);
  return 0;
}