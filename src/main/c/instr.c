#include "instr.h"
#include <stdlib.h>
#include <string.h>

/*
 * Copies the entire line to freshly allocated memory,
 * builds a new instance of `instr` with `label` and `jump`
 * set to NULL.
 */
instr instr_new(char* line) {
  instr res = malloc(sizeof(struct instr));
  res->jump = (char*)NULL;
  res->label = (char*)NULL;
  int n = strlen(line);
  res->line = (char*) malloc((n + 1) * sizeof(char));
  strcpy(res->line, line);
  (res->line)[n] = '\0';
  res->isUnconditionalJump = false;
  res->isReturn = false;
  return res;
}

/**
 * Frees label, jump, line-string, and the instruction structure.
 */
void instr_free(instr i) {
  if (i) {
    if (i->jump) {
      free(i->jump);
    }
    if (i->label) {
      free(i->label);
    }
    free(i->line);
    free(i);
  }
}

/* Copies the label, saves it as member of the instruction. */
void instr_set_label(instr i, char* lbl, int lbl_len) {
  i->label = (char*) malloc((lbl_len + 1) * sizeof(char));
  (i->label)[lbl_len] = '\0';
  strncpy(i->label, lbl, lbl_len);
}

/*
 * Copies the `jump` string to freshly allocated memory,
 * sets the corresponding member of the `i` to point to
 * this new string.
 */
void instr_set_jump(instr i, char* jmp, int jmp_len) {
  i->jump = (char*) malloc((jmp_len + 1) * sizeof(char));
  (i->jump)[jmp_len] = '\0';
  strncpy(i->jump, jmp, jmp_len);
}

