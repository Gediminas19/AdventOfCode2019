#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "intcode_comp.h"

int main() {
  int len;
  scanf("%d\n", &len);

  long *work = calloc(len, sizeof(long));
  for (int j = 0; j < len; j++) scanf("%li,", &work[j]);

  chain *master_buffer_queue = calloc(1, sizeof(chain));
  intcode_comp **comps = calloc(50, sizeof(intcode_comp*));
  for (int j = 0; j < 50; j++) {
    comps[j] = init_comp(work, len);
    add_input(comps[j], j);
  }
  long natx = 0, naty = 0;
  long sentx = 0, senty = 0;
  bool idle, previdle = false;

  int iter = 0;
  while (true) {
    printf("Iteration %d::\n", iter);

    for (int j = 0; j < 50; j++) {
      run(comps[j]);
      while (more_outputs(comps[j]))
        enq(master_buffer_queue, get_output(comps[j]));
    }

    idle = true;

    while (!chain_empty(master_buffer_queue)) {
      idle = false;

      int dest = deq(master_buffer_queue);
      long x = deq(master_buffer_queue);
      long y = deq(master_buffer_queue);

      printf("Send %ld,%ld to comp %2d\n", x, y, dest);

      if (dest == 255) {
        printf("NAT now contains %ld,%ld\n", x, y);
        natx = x;
        naty = y;
      } else {
        add_input(comps[dest], x);
        add_input(comps[dest], y);
      }
    }

    if (idle && previdle) {
      printf("NAT sends %ld,%ld to address 0\n", natx, naty);
      add_input(comps[0], natx);
      add_input(comps[0], naty);

      if (natx == sentx && naty == senty) {
        puts("SENT BEFORE");
        return 0;
      }
      sentx = natx;
      senty = naty;
    } else {
      for (int j = 0; j < 50; j++) {
        add_input(comps[j], -1);
      }
    }
    previdle = idle;

    iter++;
    puts("");
  }

  free(work);
  // free_comp(comp);
  return 0;
}