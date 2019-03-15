#include "cfg_renderer.h"
#include "instr.h"
#include "hash.h"

void cfg_render(cfg_renderer r, list bblocks) {
  // this keeps track of the nodes that actually
  // do exist.
  hash spottedNodeLabels = hash_new(
    string_hashcode,
    string_equals
  );

  // start drawing the graph, render all nodes,
  // create set of nodes that actually do exist 
  // along the way
  r->enterGraph();
  foreach_list(bblock, b, bblocks) {
    hash_put(spottedNodeLabels, (void *)b->id, (void *)(long)1);
    r->enterNode(b->id);
    foreach_list(instr, i, b->instructions) {
      r->renderInstruction(i->line);
    }
    r->exitNode();
  }

  // render the fallthrough-edges
  bool isFirst = true;
  char* prevId;
  bool noFallthrough = false;
  foreach_list(bblock, b, bblocks) {
    if (!isFirst && !noFallthrough) {
      char * nextId = b->id;
      r->renderEdge(prevId, nextId);
    }
    isFirst = false;
    prevId = b->id;
    noFallthrough = !bblock_canFallthrough(b);
  }

  // render the jump-edges,
  // check that the nodes actually do exist before
  // drawing edges to them
  foreach_list(bblock, b, bblocks) {
    char * jumpSource = b->id;
    char * jumpTarget = bblock_getExitJump(b);
    if (jumpTarget) {
      if (hash_contains(spottedNodeLabels, jumpTarget)) {
        r->renderEdge(jumpSource, jumpTarget);
      }
    }
  }

  // finish graph
  r->exitGraph();
}