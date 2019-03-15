#include "list.h"
#include <assert.h>

typedef void * voidptr;

list list_empty() {
  return NULL;
}

list list_cons(void *head, list tail) {
  list res = malloc(sizeof(struct list));
  res->head = head;
  res->tail = tail;
  return res;
}

void list_free(list l) {
  if (l) {
    list t = l->tail;
    free(l);
    list_free(t);
  }
}

bool list_isEmpty(list l) {
  return l == NULL;
}

int list_size(list l) {
  int c = 0;
  foreach_list(voidptr, i, l) {
    c++;
  }
  return c;
}

void * list_head(list l) {
  return l->head;
}

list list_tail(list l) {
  return l->tail;
}

void * list_last(list l) {
  assert(l);
  if (l->tail == NULL) {
    return l->head;
  } else {
    return list_last(l->tail);
  }
}

list list_reverse(list l) {
  list res = list_empty();
  foreach_list(voidptr, i, l) {
    res = list_cons(i, res);
  }
  return res;
}
