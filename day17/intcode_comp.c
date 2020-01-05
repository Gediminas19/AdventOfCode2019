#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "intcode_comp.h"

wrap *new_wrap(long input) {
  wrap *res = calloc(1, sizeof(wrap));
  res->input = input;
  return res;
}

void enq(chain *inputs, long input) {
  if (inputs->start == NULL) {
    inputs->start = new_wrap(input);
    inputs->end = inputs->start;
    return;
  }
  inputs->end->next = new_wrap(input);
  inputs->end = inputs->end->next;
}

long deq(chain *inputs) {
  long res = inputs->start->input;
  wrap *to_free = inputs->start;
  inputs->start = inputs->start->next;
  free(to_free);
  return res;
}

bool chain_empty(chain *inputs) {
  return inputs->start == NULL;
}

void add_input(intcode_comp *amp, long input) {
  enq(amp->inputs, input);
}

bool more_outputs(intcode_comp *amp) {
  return !chain_empty(amp->outputs);
}

long get_output(intcode_comp *amp) {
  return deq(amp->outputs);
}

intcode_comp *init_comp(long *work, int len) {
  intcode_comp *comp = calloc(1, sizeof(intcode_comp));
  comp->pc = 0;
  comp->relbase = 0;
  comp->code = calloc(200000, sizeof(long));
  memcpy(comp->code, work, len*sizeof(long));
  comp->inputs = calloc(1, sizeof(chain));
  comp->outputs = calloc(1, sizeof(chain));
  return comp;
}

void set_params(intcode_comp *amp, long **params, int paramcount) {
  long temp = amp->code[amp->pc]/100;
  for (int i = 0; i < paramcount; i++) {
    int paramode = temp % 10;
    if (paramode == 0) params[i] = &amp->code[amp->code[amp->pc + 1 + i]];
    else if (paramode == 2) params[i] = &amp->code[amp->relbase + amp->code[amp->pc + 1 + i]];
    else if (paramode == 1) params[i] = &amp->code[amp->pc + 1 + i];
    temp /= 10;
  }
}

bool run(intcode_comp *amp) {
  long **params = calloc(3, sizeof(long*));
  while (amp->code[amp->pc] != 99) {
    // printf("pc %d: %ld\n", amp->pc, amp->code[amp->pc]);
    switch (amp->code[amp->pc] % 100) {
      case 1:
        set_params(amp, params, 3);
        *params[2] = *params[0] + *params[1];
        amp->pc += 4;
        break;
      case 2:
        set_params(amp, params, 3);
        *params[2] = *params[0] * *params[1];
        amp->pc += 4;
        break;
      case 3:
        if (chain_empty(amp->inputs)) return true;
        set_params(amp, params, 1);
        *params[0] = deq(amp->inputs);
        amp->pc += 2;
        break;
      case 4:
        set_params(amp, params, 1);
        amp->pc += 2;
        enq(amp->outputs, *params[0]);
        break;
      case 5:
        set_params(amp, params, 2);
        if (*params[0] != 0) amp->pc = *params[1];
        else amp->pc += 3;
        break;
      case 6:
        set_params(amp, params, 2);
        if (*params[0] == 0) amp->pc = *params[1];
        else amp->pc += 3;
        break;
      case 7:
        set_params(amp, params, 3);
        *params[2] = *params[0] < *params[1]? 1 : 0;
        amp->pc += 4;
        break;
      case 8:
        set_params(amp, params, 3);
        *params[2] = *params[0] == *params[1]? 1 : 0;
        amp->pc += 4;
        break;
      case 9:
        set_params(amp, params, 1);
        amp->relbase += *params[0];
        amp->pc += 2;
    }
  }
  return false;
}