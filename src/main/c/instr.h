#ifndef INSTR_H
#define INSTR_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * Data structure to represent a single 
 * instruction (i.e. single line of ahn
 * assembly-like file).
 */
struct instr {
  /* Entire line. */
  char *line;
  /* Optional extracted label (can be NULL). */
  char *label;
  /* Optional extracted jump target (can be NULL). */
  char *jump;
  /* Checks whether the instruction is a return */
  bool isReturn;
  /* Checks whether the instruction is an unconditional jump (no fallthrough) */
  bool isUnconditionalJump;
};

typedef struct instr * instr;

/*
 * Copies the entire line to freshly allocated memory,
 * builds a new instance of `instr` with `label` and `jump`
 * set to NULL.
 */
instr instr_new(char* line);

/* Copies the label, saves it as member of the instruction. */
void instr_set_label(instr i, char* lbl, int lbl_len);

/* Copies the `jump` string, saves it as member of the instruction. */
void instr_set_jump(instr i, char* jmp, int jmp_len);

#endif

