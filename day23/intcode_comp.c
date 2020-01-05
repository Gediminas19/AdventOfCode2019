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

void add_input(intcode_comp *comp, long input) {
  enq(comp->inputs, input);
}

bool more_outputs(intcode_comp *comp) {
  return !chain_empty(comp->outputs);
}

long get_output(intcode_comp *comp) {
  return deq(comp->outputs);
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

void reset_comp(intcode_comp *comp, long *work, int len) {
  comp->pc = 0;
  comp->relbase = 0;
  memset(comp->code, 0, 200000*sizeof(long));
  memcpy(comp->code, work, len*sizeof(long));
  while (!chain_empty(comp->inputs)) deq(comp->inputs);
  while (!chain_empty(comp->outputs)) deq(comp->outputs);
}

void free_comp(intcode_comp *comp) {
  free(comp->code);
  while (!chain_empty(comp->inputs)) deq(comp->inputs);
  free(comp->inputs);
  while (!chain_empty(comp->outputs)) deq(comp->outputs);
  free(comp->outputs);
  free(comp);
}

void set_params(intcode_comp *comp, long **params, int paramcount) {
  long temp = comp->code[comp->pc]/100;
  for (int i = 0; i < paramcount; i++) {
    int paramode = temp % 10;
    if (paramode == 0) params[i] = &comp->code[comp->code[comp->pc + 1 + i]];
    else if (paramode == 2) params[i] = &comp->code[comp->relbase + comp->code[comp->pc + 1 + i]];
    else if (paramode == 1) params[i] = &comp->code[comp->pc + 1 + i];
    temp /= 10;
  }
}

int step(intcode_comp *comp) {
  long *params[3];
  switch (comp->code[comp->pc] % 100) {
    case 1:
      set_params(comp, params, 3);
      *params[2] = *params[0] + *params[1];
      comp->pc += 4;
      return 0;
    case 2:
      set_params(comp, params, 3);
      *params[2] = *params[0] * *params[1];
      comp->pc += 4;
      return 0;
    case 3:
      if (chain_empty(comp->inputs)) return 1;
      set_params(comp, params, 1);
      *params[0] = deq(comp->inputs);
      comp->pc += 2;
      return 0;
    case 4:
      set_params(comp, params, 1);
      comp->pc += 2;
      enq(comp->outputs, *params[0]);
      return 2;
    case 5:
      set_params(comp, params, 2);
      if (*params[0] != 0) comp->pc = *params[1];
      else comp->pc += 3;
      return 0;
    case 6:
      set_params(comp, params, 2);
      if (*params[0] == 0) comp->pc = *params[1];
      else comp->pc += 3;
      return 0;
    case 7:
      set_params(comp, params, 3);
      *params[2] = *params[0] < *params[1]? 1 : 0;
      comp->pc += 4;
      return 0;
    case 8:
      set_params(comp, params, 3);
      *params[2] = *params[0] == *params[1]? 1 : 0;
      comp->pc += 4;
      return 0;
    case 9:
      set_params(comp, params, 1);
      comp->relbase += *params[0];
      comp->pc += 2;
      return 0;
    case 99:
      comp->pc += 1;
      return -1;
  }
}

bool run(intcode_comp *comp) {
  int status;
  while (abs(status = step(comp)) != 1);
  return status == 1;
}