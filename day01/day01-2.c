#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  FILE *fp = fopen("input.txt", "r");
  int mass;
  int total_fuel = 0;
  while(fscanf(fp, "%d\n", &mass) != EOF) {
    int fuel = mass/3 - 2;
    while (fuel > 0) {
      total_fuel += fuel;
      fuel = fuel/3 - 2;
    }
  }
  fclose(fp);
  printf("Part 2: %d\n", total_fuel);

  return 0;
}