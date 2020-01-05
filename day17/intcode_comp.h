#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef INTCODE_H
#define INTCODE_H

typedef struct wrap wrap;
struct wrap {
  long input;
  wrap *next;
};

typedef struct chain chain;
struct chain {
  wrap *start;
  wrap *end;
};

wrap *new_wrap(long input);

void enq(chain *inputs, long input);
long deq(chain *inputs);

typedef struct intcode_comp intcode_comp;
struct intcode_comp {
  long *code;
  int pc;
  long relbase;
  chain *inputs;
  chain *outputs;
};

void add_input(intcode_comp *amp, long input);
bool more_outputs(intcode_comp *amp);
long get_output(intcode_comp *amp);

intcode_comp *init_comp(long *work, int len);
void set_params(intcode_comp *amp, long **params, int paramcount);
bool run(intcode_comp *amp);

#endif