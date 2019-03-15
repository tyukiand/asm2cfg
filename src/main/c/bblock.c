#include "bblock.h"
#include <assert.h>
#include <string.h>
#include "hash.h"
#include <stdio.h>

/**
 * The makes a copies the id string.
 * It does own the list structure that holds the instructions.
 * It does not own the instructions: it only groups them together.
 */
bblock bblock_new(
  char * id /* copied in constructor */, 
  list instructions /* ownership transferred to the new bblock */
) {
  assert(!list_isEmpty(instructions));
  bblock res = malloc(sizeof(struct bblock));
  res->id = strdup(id);
  res->instructions = instructions;
  return res;
}

void bblock_free(bblock b) {
  free(b->id);
  list_free(b->instructions);
  free(b);
}

static hash find_jump_targets(list instructions) {
  hash res = hash_new(string_hashcode, string_equals);
  foreach_list(instr, i, instructions) {
    if (i->jump) {
      hash_put(res, (void *) i->jump, (void *)(long)true);
    }
  }
  return res;
}

/** 
 * Helper method for `group_into_basic_blocks`.
 */
static void commit_block(
  int * fallthroughIdx, 
  list instrRevAcc,
  list * revBlocks
) {
  list instrAcc = list_reverse(instrRevAcc);
  list_free(instrRevAcc);
  char * newBlockId;
  bool ownBlockId = false;
  char * firstLabel = ((instr) list_head(instrAcc))->label;
  if (firstLabel) {
    newBlockId = firstLabel;
    ownBlockId = false;
  } else {
    if (asprintf(&newBlockId, "fallthrough_%d", *fallthroughIdx) < 0) {
      printf("Fatal error: asprintf died. \n");
      exit(-1);
    }
    (*fallthroughIdx)++;
    ownBlockId = true;
  }
  bblock b = bblock_new(newBlockId, instrAcc);
  if (ownBlockId) {
    free(newBlockId);
  }
  *revBlocks = list_cons(b, *revBlocks);
}

list group_into_basic_blocks(list instructions) {
  int fallthroughIdx = 0;
  hash jumpTargets = find_jump_targets(instructions);

  bool currentIsLeader = false;
  bool previousIsJump = false;
  list revBlocks = list_empty();
  list instrRevAcc = NULL;
  foreach_list(instr, i, instructions) {
    if (i->label) {
      currentIsLeader = hash_contains(jumpTargets, (void *)i->label);
    }
    if (currentIsLeader || previousIsJump) {
      if (instrRevAcc != NULL) {
        commit_block(&fallthroughIdx, instrRevAcc, &revBlocks);
      }
      instrRevAcc = list_empty();
    }
    currentIsLeader = false;
    previousIsJump = false;
    if (i->jump) {
      previousIsJump = true;
    }
    instrRevAcc = list_cons(i, instrRevAcc);
  }

  // commit last block
  if (!list_isEmpty(instrRevAcc)) {
    commit_block(&fallthroughIdx, instrRevAcc, &revBlocks);
  }

  hash_free(jumpTargets);
  list blocks = list_reverse(revBlocks);
  list_free(revBlocks);
  return blocks;
}

static char ENTRY_LABEL[] = "ENTRY";

char * bblock_getEntryLabel(bblock b) {
  instr i = list_head(b->instructions);
  if (i->label) {
    return i->label;
  } else {
    return ENTRY_LABEL;
  }
}

char * bblock_getExitJump(bblock b) {
  instr i = list_last(b->instructions);
  if (i->jump) {
    return i->jump;
  } else {
    return NULL;
  }
}

bool bblock_canFallthrough(bblock b) {
  instr lastInstr = list_last(b->instructions);
  return !(lastInstr->isUnconditionalJump || lastInstr->isReturn);
}