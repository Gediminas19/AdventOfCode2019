#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  FILE *fp = fopen("input.txt", "r");
  int mass;
  int total_fuel = 0;
  while(fscanf(fp, "%d\n", &mass) != EOF) {
    total_fuel += mass/3 - 2;
  }
  fclose(fp);
  printf("Part 1: %d\n", total_fuel);

  return 0;
}