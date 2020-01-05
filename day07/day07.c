#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct input_wrap input_wrap;
struct input_wrap {
  int input;
  input_wrap *next;
};

input_wrap *new_input(int input) {
  input_wrap *res = calloc(1, sizeof(input_wrap));
  res->input = input;
  return res;
}

typedef struct input_chain input_chain;
struct input_chain {
  input_wrap *start;
  input_wrap *end;
};

void enq(input_chain *inputs, int input) {
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

int deq(input_chain *inputs) {
  int res = inputs->start->input;
  input_wrap *to_free = inputs->start;
  inputs->start = inputs->start->next;
  free(to_free);
  return res;
}

typedef struct intcode_comp intcode_comp;
struct intcode_comp {
  int *code;
  int pc;
  input_chain *inputs;
  int code_len;
};

int run(intcode_comp *amp, bool *jenga);

int factorial(int n) {
  if (n == 0) return 1;
  return n*factorial(n - 1);
}

void permute(int **output, int *a, int len) {
  int i, j;
  int f=factorial(len);     //factorial
  int c;          //buffer
  while (f--) {
    output[f] = calloc(len, sizeof(int));
    for (int q = 0; q < len; q++) {
      output[f][q] = a[q];
      if (a[q] == 5) puts("HARk");
    }
    // printf("%s\n", a);
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

int main()
{
  int len;
  scanf("%d\n", &len);

  int *work = calloc(len, sizeof(int));
  for (int j = 0; j < len; j++) scanf("%d,", &work[j]);

  int *phases = calloc(5, sizeof(int));
  for (int j = 0; j < 5; j++) phases[j] = j;

  int f = factorial(5);
  int **perms = calloc(f, sizeof(int*));
  permute(perms, phases, 5);
  for (int j = 0; j < f; j++) for (int k = 0; k < 5; k++) perms[j][k] += 5;

  // for (int j = 0; j < len; j++) {
  //   printf("%d ", work[j]);
  // }
  // puts("");

  intcode_comp **amps = calloc(5, sizeof(intcode_comp*));
  for (int j = 0; j < 5; j++) {
    amps[j] = calloc(1, sizeof(intcode_comp));
    amps[j]->pc = 0;
    amps[j]->code = calloc(len, sizeof(int));
    amps[j]->code_len = len;
    memcpy(amps[j]->code, work, len*sizeof(int));
    amps[j]->inputs = calloc(1, sizeof(input_chain));
  }

  //Part 1
  int maxThrust = -100;

  for (int j = 0; j < f; j++) {
    // int j = 0;
    // int darth[] = {9, 8, 7, 6, 5};
    printf("Currently testing ");
    for (int k = 0; k < 5; k++) printf("%d", perms[j][k]);// = darth[k];
    puts("");
    input_chain **inputs = calloc(5, sizeof(input_chain*));
    for (int k = 0; k < 5; k++) {
      // printf("%d", perms[j][k]);
      enq(amps[k]->inputs, perms[j][k]);
    }
    // puts("");
    enq(amps[0]->inputs, 0);

    bool jenga = true;
    bool freg = true;
    int e = 0;
    int preve = 0;
    while (true) {
      // puts("Amp A: ");
      int a = run(amps[0], &jenga);
      enq(amps[1]->inputs, a);
      // puts("Amp B: ");
      int b = run(amps[1], &jenga);
      enq(amps[2]->inputs, b);
      // puts("Amp C: ");
      int c = run(amps[2], &jenga);
      enq(amps[3]->inputs, c);
      // puts("Amp D: ");
      int d = run(amps[3], &jenga);
      enq(amps[4]->inputs, d);
      // puts("Amp E: ");
      if (!jenga) {
        e = preve;
        break;
      }
      e = run(amps[4], &jenga);
      enq(amps[0]->inputs, e);
      if (!jenga) {
        e = preve;
        break;
      }
      // printf("Tempura: %d\n", e);
      preve = e;
    }
    printf("Thrust: %d\n", e);
    if (e > maxThrust) maxThrust = e;

    for (int k = 0; k < 5; k++) {
      amps[k]->pc = 0;
      memcpy(amps[k]->code, work, len*sizeof(int));
      amps[k]->inputs = calloc(1, sizeof(input_chain));
    }
  }
  printf("MaxThrust: %d\n", maxThrust);

  free(work);
  return 0;
}

int run(intcode_comp *amp, bool *jenga) {
  int opcode = -1;
  int params[3];
  while (amp->code[amp->pc] != 99) {
    // printf("pc %d: %d\n", amp->pc, amp->code[amp->pc]);
    opcode = amp->code[amp->pc] % 100;
    int temp = amp->code[amp->pc]/100;
    for (int i = 0; i < 3; i++) {
      if (temp % 10 == 0 && amp->code[amp->pc + 1 + i] < amp->code_len) params[i] = amp->code[amp->code[amp->pc + 1 + i]];
      else params[i] = amp->code[amp->pc + 1 + i];
      temp /= 10;
      // printf("param %d: %d\n", i, params[i]);
    }

    switch (opcode) {
      case 1:
        amp->code[amp->code[amp->pc + 3]] = params[0] + params[1];
        amp->pc += 4;
        break;
      case 2:
        amp->code[amp->code[amp->pc + 3]] = params[0]*params[1];
        amp->pc += 4;
        break;
      case 3: {
        int in = deq(amp->inputs);
        // printf("input %d\n", in);
        amp->code[amp->code[amp->pc + 1]] = in;
        amp->pc += 2;
        break;
      }
      case 4:
        // printf("output %d\n\n", params[0]);
        amp->pc += 2;
        return params[0];
        break;
      case 5:
        if (params[0] != 0) amp->pc = params[1];
        else amp->pc += 3;
        break;
      case 6:
        if (params[0] == 0) amp->pc = params[1];
        else amp->pc += 3;
        break;
      case 7:
        amp->code[amp->code[amp->pc + 3]] = params[0] < params[1]? 1 : 0;
        amp->pc += 4;
        break;
      case 8:
        amp->code[amp->code[amp->pc + 3]] = params[0] == params[1]? 1 : 0;
        amp->pc += 4;
        break;
    }
  }
  // puts("Hattin");
  *jenga = false;
  return -1;
}