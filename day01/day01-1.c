#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int mass;
  int total_fuel = 0;
  while(scanf("%d\n", &mass) != EOF) {
    total_fuel += mass/3 - 2;
  }
  printf("Part 1: %d\n", total_fuel);

  return 0;
}