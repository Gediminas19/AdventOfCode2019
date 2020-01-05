#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct input_wrap input_wrap;
struct input_wrap {
  long input;
  input_wrap *next;
};

input_wrap *new_input(long input) {
  input_wrap *res = calloc(1, sizeof(input_wrap));
  res->input = input;
  return res;
}

typedef struct input_chain input_chain;
struct input_chain {
  input_wrap *start;
  input_wrap *end;
};

void enq(input_chain *inputs, long input) {
  // printf("oui %d ", input);
  if (inputs->start == NULL) {
    // puts("nuevo");
    inputs->start = new_input(input);
    inputs->end = inputs->start;
    return;
  }
  // puts("old");
  inputs->end->next = new_input(input);
  inputs->end = inputs->end->next;
}

long deq(input_chain *inputs) {
  long res = inputs->start->input;
  input_wrap *to_free = inputs->start;
  inputs->start = inputs->start->next;
  free(to_free);
  return res;
}

typedef struct intcode_comp intcode_comp;
struct intcode_comp {
  long *code;
  int pc;
  long relbase;
  input_chain *inputs;
};

long run(intcode_comp *amp);

int main()
{
  int len;
  scanf("%d\n", &len);

  long *work = calloc(len, sizeof(long));
  for (int j = 0; j < len; j++) scanf("%li,", &work[j]);

  intcode_comp *comp = calloc(1, sizeof(intcode_comp));
  comp->pc = 0;
  comp->relbase = 0;
  comp->code = calloc(100000, sizeof(long));
  memcpy(comp->code, work, len*sizeof(long));
  comp->inputs = calloc(1, sizeof(input_chain));

  enq(comp->inputs, 2);
  run(comp);

  free(work);
  free(comp);
  return 0;
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

long run(intcode_comp *amp) {
  int res = -1;
  long opcode = -1;
  long **params = calloc(3, sizeof(long*));
  while (amp->code[amp->pc] != 99) {
    // printf("pc %d: %ld\n", amp->pc, amp->code[amp->pc]);
    opcode = amp->code[amp->pc] % 100;

    switch (opcode) {
      case 1:
        set_params(amp, params, 3);
        *params[2] = *params[0] + *params[1];
        // printf("Stored %ld to position %ld\n", *params[0] + *params[1], amp->code[amp->pc + 3]);
        amp->pc += 4;
        break;
      case 2:
        set_params(amp, params, 3);
        *params[2] = *params[0] * *params[1];
        // printf("Position %ld now contains %ld\n", amp->code[amp->pc + 3], amp->code[amp->code[amp->pc + 3]]);
        amp->pc += 4;
        break;
      case 3: {
        set_params(amp, params, 1);
        long in = deq(amp->inputs);
        printf("input %ld\n", in);
        *params[0] = in;
        amp->pc += 2;
        break;
      }
      case 4:
        set_params(amp, params, 1);
        printf("output %ld\n\n", *params[0]);
        amp->pc += 2;
        res = *params[0];
        // return *params[0];
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
  // puts("Hattin");
  return res;
}