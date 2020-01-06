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
void deq(chain *inputs);
long peek(chain *inputs);
bool chain_empty(chain *inputs);

typedef struct intcode_comp intcode_comp;
struct intcode_comp {
  long *code;
  int pc;
  long relbase;
  chain *inputs;
  chain *outputs;
};

void add_input(intcode_comp *comp, long input);
bool more_outputs(intcode_comp *comp);
long get_output(intcode_comp *comp);
long peek_output(intcode_comp *comp);

void ascii_in(intcode_comp *comp, char *input);
void ascii_in_ln(intcode_comp *comp, char *input);
void ascii_out(intcode_comp *comp);

intcode_comp *init_comp(long *work, int len);
void reset_comp(intcode_comp *comp, long *work, int len);
void free_comp(intcode_comp *comp);
void set_params(intcode_comp *comp, long **params, int paramcount);
int step(intcode_comp *comp);
bool run(intcode_comp *comp);

#endif