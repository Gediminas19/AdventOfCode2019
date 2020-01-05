  #include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char c;
  char data[100][150];
  for (int j = 0; j < 100; j++) {
    for (int k = 0; k < 150; k++) {
      scanf("%c", &c);
      data[j][k] = c;
    }
  }

  int minceros = 150, mincerow = -1;
  for (int j = 0; j < 100; j++) {
    int ceros = 0;
    for (int k = 0; k < 150; k++) {
      if (data[j][k] == '0') ceros++;
    }
    if (ceros < minceros) {
      minceros = ceros;
      mincerow = j;
    }
  }

  int unos = 0, doss = 0;
  for (int k = 0; k < 150; k++) {
    if (data[mincerow][k] == '1') unos++;
    else if (data[mincerow][k] == '2') doss++;
  }
  printf("Part 1: %d\n", unos*doss);

  char viewed[150];
  for (int k = 0; k < 150; k++) {
    viewed[k] = '2';
    for (int j = 0; j < 100; j++) {
      if (data[j][k] != '2') {
        viewed[k] = data[j][k];
        break;
      }
    }
  }

  puts("Part 2: ");
  for (int m = 0; m < 6; m++) {
    for (int n = 0; n < 25; n++) {
      if (viewed[m*25 + n] == '0') printf("\u25a0");
      else printf(" ");
    }
    puts("");
  }

  return 0;
}