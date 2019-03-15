#ifndef CFG_RENDERER_H
#define CFG_RENDERER_H

#include "instr.h"
#include "bblock.h"

struct cfg_renderer {
  void (*enterGraph)(void);
  void (*enterNode)(char * nodeId);
  void (*renderInstruction)(char * instructionLine);
  void (*exitNode)(void);
  void (*renderEdge)(char * fromNodeId, char * toNodeId);
  void (*exitGraph)(void);
};

typedef struct cfg_renderer * cfg_renderer;

void cfg_render(cfg_renderer renderer, list basicBlocks);

#endif