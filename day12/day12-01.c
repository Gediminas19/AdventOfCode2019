#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // int moonpos[4][3] = {{-1, 0, 2}, {2, -10, -7}, {4, -8, 8}, {3, 5, -1}};
  int moonpos[4][3] = {{12, 0, -15}, {-8, -5, -10}, {7, -17, 1}, {2, -11, -6}};
  int moonvel[4][3] = {{0}};

  int iters = 1000;
  for (int j = 0; j < iters; j++) {
    // Debug
    printf("Iteration %d:\n", j);
    for (int p = 0; p < 4; p++) {
      for (int r = 0; r < 3; r++) {
        printf("%d ", moonpos[p][r]);
      }
      printf(" : ");
      for (int r = 0; r < 3; r++) {
        printf("%d ", moonvel[p][r]);
      }
      puts("");
    }
    // Apply gravity to velocity
    for (int p = 0; p < 3; p++) {
      for (int q = p + 1; q < 4; q++) {
        for (int r = 0; r < 3; r++) {
          int delta = (moonpos[p][r] < moonpos[q][r])? 1 : ((moonpos[p][r] == moonpos[q][r])? 0 : -1);
          moonvel[p][r] += delta;
          moonvel[q][r] -= delta;
        }
      }
    }
    // Apply velocity to position
    for (int p = 0; p < 4; p++) {
      for (int r = 0; r < 3; r++) {
        moonpos[p][r] += moonvel[p][r];
      }
    }
  }

  // Calculate energy
  int energy = 0;
  for (int p = 0; p < 4; p++) {
    int pot = 0, kin = 0;
    for (int r = 0; r < 3; r++) {
      pot += abs(moonpos[p][r]);
      kin += abs(moonvel[p][r]);
    }
    energy += pot*kin;
  }
  printf("%d\n", energy);

  return 0;
}