#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../intcode_comp/intcode_comp.h"

int comp(const void * a, const void * b) {
   return *(int*)b - *(int*)a;
}

void permute5(int **output, int *a) {
  // first sort from greatest to least
  qsort(a, 5, sizeof(int), &comp);
  // because the strange code below only works when a is sorted great to least
  int i, j;
  int f = 120;     //factorial
  int c;          //buffer
  while (f--) {
    output[f] = calloc(5, sizeof(int));
    memcpy(output[f], a, 5*sizeof(int));

    i=1;
    while(a[i] > a[i-1]) i++;
    j=0;
    while(a[j] < a[i]) j++;
    c=a[j];
    a[j]=a[i];
    a[i]=c;
    i--;
    for (j = 0; j < i; i--, j++) {
      c = a[i];
      a[i] = a[j];
      a[j] = c;
    }
  }
}

int main() {
  int len;
  long *work = parse_input("input.txt", &len);

  int *phases = calloc(5, sizeof(int));
  for (int j = 0; j < 5; j++) phases[j] = j;

  int **perms = calloc(120, sizeof(int*));
  permute5(perms, phases);

  intcode_comp **amps = calloc(5, sizeof(intcode_comp*));
  for (int j = 0; j < 5; j++) amps[j] = init_comp(work, len);

  // Part 1
  int maxThrust = INT_MIN;

  for (int j = 0; j < 120; j++) {
    for (int k = 0; k < 5; k++) add_input(amps[k], perms[j][k]);

    add_input(amps[0], 0);
    run(amps[0]);
    add_input(amps[1], get_output(amps[0]));
    run(amps[1]);
    add_input(amps[2], get_output(amps[1]));
    run(amps[2]);
    add_input(amps[3], get_output(amps[2]));
    run(amps[3]);
    add_input(amps[4], get_output(amps[3]));
    run(amps[4]);

    int thrust = get_output(amps[4]);
    if (thrust > maxThrust) maxThrust = thrust;

    for (int k = 0; k < 5; k++) reset_comp(amps[k], work, len);
  }
  printf("No Feedback MaxThrust: %d\n", maxThrust);


  // Part 2
  for (int j = 0; j < 5; j++) phases[j] = j + 5;
  permute5(perms, phases);

  maxThrust = INT_MIN;

  for (int j = 0; j < 120; j++) {
    for (int k = 0; k < 5; k++) add_input(amps[k], perms[j][k]);

    int thrust = 0;
    bool ongoing = true;
    while (ongoing) {
      add_input(amps[0], thrust);
      run(amps[0]);
      add_input(amps[1], get_output(amps[0]));
      run(amps[1]);
      add_input(amps[2], get_output(amps[1]));
      run(amps[2]);
      add_input(amps[3], get_output(amps[2]));
      run(amps[3]);
      add_input(amps[4], get_output(amps[3]));
      ongoing = run(amps[4]);

      thrust = get_output(amps[4]);
    }
    if (thrust > maxThrust) maxThrust = thrust;

    for (int k = 0; k < 5; k++) reset_comp(amps[k], work, len);
  }
  printf("With Feedback MaxThrust: %d\n", maxThrust);

  free(work);
  for (int j = 0; j < 5; j++) free_comp(amps[j]);
  return 0;
}