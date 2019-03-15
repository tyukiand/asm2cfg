#include "instr.h"
#include "instr_list.h"
#include <stdio.h>
#include <stdbool.h>

instr_list instr_list_empty() {
  instr_list res = (instr_list) malloc(sizeof(struct instr_list));
  res->isEmpty = true;
  res->head;
  res->tail = (instr_list) NULL;
  return res;
}

instr_list instr_list_cons(struct instr i, instr_list tail) {
  instr_list res = (instr_list) malloc(sizeof(struct instr_list));
  res->isEmpty = false;
  res->head = i;
  res->tail = tail;
  return res;
}

void instr_list_free(instr_list instructions) {
  instr_list ht;
  instr_list nil;
  for (ht = instructions; ht->tail; ht = ht->tail) {
    nil = ht->tail;
    free((void*)ht);
  }
  free((void*)nil);
}

void instr_list_foreach(instr_list instructions, void (*action)(struct instr)) {
  for (instr_list ht = instructions; ht->tail; ht = ht->tail) {
    (*action)(ht->head);
  }
}

instr_list instr_list_reverse(instr_list instructions) {
  instr_list result = instr_list_empty();
  for (instr_list i = instructions; i->tail; i = i->tail) {
    result = instr_list_cons(i->head, result);
  }
  return result;
}

unsigned int instr_list_size(instr_list instructions) {
  unsigned int res = 0;
  for (instr_list i = instructions; i->tail; i = i->tail, res++);
  return res;
}

// void printInstrLine(struct instr i) {
//   printf("%s\n", i.line);
// }
// 
// int main(int argc, char** args) {
//   struct instr a = instr_new("hello, world");
//   struct instr b = instr_new("goodbye for now");
//   struct instr c = instr_new("goodbye for now so long");
//   
//   instr_list l = 
//    instr_list_cons(a, 
//      instr_list_cons(b,
//        instr_list_cons(c,
//          instr_list_empty())));
// 
//   
//   instr_list_foreach(l, &printInstrLine);
// 
//   instr_list rev = instr_list_reverse(l);
//   instr_list_free(l);
// 
//   instr_list_foreach(rev, &printInstrLine);
//   instr_list_free(rev);
//   return 0;
// }
