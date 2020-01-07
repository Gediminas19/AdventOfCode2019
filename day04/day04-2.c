#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main() {
  int start, end;

  FILE *fp = fopen("input.txt", "r");
  fscanf(fp, "%d-%d", &start, &end);
  fclose(fp);

  int success = 0;
  for (int num = start; num <= end; num++) {
    bool doublon = false, unmonotone = false;
    int doubled = -1;
    int prevdig = num%10;
    for (int j = num/10; j > 0; j /= 10) {
      // printf("%d %d\n", prevdig, j%10);
      if (prevdig == j%10) {
        if (prevdig != doubled && !doublon) {
          // printf("Aye %d\n", prevdig);
          doublon = true;
          doubled = prevdig;
        } else if (prevdig == doubled && doublon) doublon = false;
      }
      if (prevdig < j%10) {
        unmonotone = true;
        break;
      }
      prevdig = j%10;
    }
    if (unmonotone) continue;
    if (doublon) {
      // printf("%d\n", num);
      success += 1;
    }
  }
  printf("Part 2: %d\n", success);

  return 0;
}