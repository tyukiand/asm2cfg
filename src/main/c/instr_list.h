#ifndef INSTR_LIST_H
#define INSTR_LIST_H
#include "instr.h"
#include <stdbool.h>

struct instr_list {
  bool isEmpty;
  struct instr head;
  struct instr_list * tail;
};

typedef struct instr_list * instr_list;

instr_list instr_list_empty();
instr_list instr_list_cons(struct instr i, instr_list tail);
void instr_list_free(instr_list instructions);
void instr_list_foreach(instr_list instructions, void (*action)(struct instr));
instr_list instr_list_reverse(instr_list instructions);
unsigned int instr_list_size(instr_list instructions);

#endif
