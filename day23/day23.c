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

  int *output_stage = calloc(50, sizeof(int));
  chain **next_buffer_queues = calloc(50, sizeof(chain*));
  chain **buffer_queues = calloc(50, sizeof(chain*));
  intcode_comp **comps = calloc(50, sizeof(intcode_comp*));
  for (int j = 0; j < 50; j++) {
    comps[j] = init_comp(work, len);
    add_input(comps[j], j);
    buffer_queues[j] = calloc(1, sizeof(chain));
    next_buffer_queues[j] = calloc(1, sizeof(chain));
  }
  long natx = 0, naty = 0;
  long sentx = 0, senty = 0;
  int idle_for = -1;

  int iter = 0;
  while (true) {
    // printf("Iteration %d\n", iter++);

    for (int j = 0; j < 50; j++) {
      // printf("Comp %2d running %2d\n", j, (int) comps[j]->code[comps[j]->pc] % 100);
      int status = step(comps[j]);
      if (status == 1) {
        if (chain_empty(buffer_queues[j])) {
          // printf("comp %2d attempted to receive\n", j);
          add_input(comps[j], -1);
        } else {
          idle_for = -1;
          // printf("comp %2d successfully received\n", j);
          add_input(comps[j], deq(buffer_queues[j]));
          add_input(comps[j], deq(buffer_queues[j]));
        }
        step(comps[j]);
      } else if (status == 2) {
        idle_for = -1;
        output_stage[j] = (output_stage[j] + 1) % 3;
        if (output_stage[j] == 0) {
          int dest = get_output(comps[j]);
          long x = get_output(comps[j]);
          long y = get_output(comps[j]);

          if (dest == 255) {
            printf("Iteration %d: NAT now contains %ld,%ld\n", iter, x, y);
            natx = x;
            naty = y;
          } else {
            enq(next_buffer_queues[dest], x);
            enq(next_buffer_queues[dest], y);
          }
        }
      }
    }

    for (int j = 0; j < 50; j++) {
      while (!chain_empty(next_buffer_queues[j]))
        enq(buffer_queues[j], deq(next_buffer_queues[j]));
      if (!chain_empty(buffer_queues[j])) idle_for = -1;
    }

    if (idle_for++ > 10000) {
      printf("Iteration %d: NAT sends %ld,%ld to address 0\n", iter, natx, naty);

      if (natx == sentx && naty == senty) {
        puts("SENT BEFORE");
        return 0;
      }

      sentx = natx;
      senty = naty;

      enq(buffer_queues[0], natx);
      enq(buffer_queues[0], naty);
      // return 0;
    }

    iter++;
  }

  free(work);
  // free_comp(comp);
  return 0;
}