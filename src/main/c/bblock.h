#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H

#include <stdbool.h>
#include "list.h"
#include "instr.h"

struct bblock {
  /** 
   * An id that uniquely identifies the block.
   * It describes the relation to other blocks,
   * and cannot be inferred from the instructions
   * alone. For example, blocks that do not 
   * start with a labeled instruction can be 
   * either entry-blocks, or fall-through-blocks.
   */
  char * id;
  list instructions;
};

typedef struct bblock * bblock;

bblock bblock_new(char * id, list instructions);
char * bblock_getId(bblock b);
/*
 * Can be NULL, because some blocks can
 * end with a return.
 */
char * bblock_getExitJump(bblock b);
list group_into_basic_blocks(list instructions);
bool bblock_canFallthrough(bblock b);

#endif
