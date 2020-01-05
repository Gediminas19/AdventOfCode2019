#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int mass;
  int total_fuel = 0;
  while(scanf("%d\n", &mass) != EOF) {
    int fuel = mass/3 - 2;
    while (fuel > 0) {
      total_fuel += fuel;
      fuel = fuel/3 - 2;
    }
  }
  printf("Part 2: %d\n", total_fuel);

  return 0;
}