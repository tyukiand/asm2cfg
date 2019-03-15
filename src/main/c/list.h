#ifndef TYCOLL_LIST_H
#define TYCOLL_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct list {
  void * head;
  struct list * tail;
};

typedef struct list * list;

list list_empty();
list list_cons(void *head, list tail);
void list_free(list l);
bool list_isEmpty(list l);
void *list_head(list lst);
list list_tail(list lst);
int list_size(list l);
list list_reverse(list l);
void *list_last(list lst);

#define foreach_list(typ, i, lst) \
  for ( \
    typ _lstCons = (typ) lst, i; \
    (_lstCons != NULL) && (i = (typ)(((list) _lstCons)->head)); \
    _lstCons = (typ)(((list) _lstCons)->tail) \
  ) \

#endif
