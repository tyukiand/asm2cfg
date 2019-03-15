#include "cfg_dot_renderer.h"
#include <stdio.h>

void dot_enterGraph() {
  printf("digraph cfg {\n");
  printf("  node["
    "fillcolor=black, fontcolor=black, "
    "fontname=courier, margin=0.1,"
    "shape=rectangle];\n"
  );
}

void dot_enterNode(char * nodeId) {
  printf("  node_%s [label=\"", nodeId);
}

void dot_renderInstruction(char * line) {
  for(char* c = line; *c; c++) {
    if (*c == '"') {
      printf("\\\"");
    } else if (*c == '\\') {
      printf("\\\\");
    } else {
      printf("%c", *c);
    }
  }
  printf("\\l");
}

void dot_exitNode() {
  printf("\"];\n");
}

void dot_renderEdge(char * fromId, char * toId) {
  printf("  node_%s -> node_%s;\n", fromId, toId);
}

void dot_exitGraph() {
  printf("}\n");
}

struct cfg_renderer DOT_RENDERER_STRUCT = (struct cfg_renderer){
  .enterGraph = dot_enterGraph,
  .enterNode = dot_enterNode,
  .renderInstruction = dot_renderInstruction,
  .exitNode = dot_exitNode,
  .renderEdge = dot_renderEdge,
  .exitGraph = dot_exitGraph
};

cfg_renderer DOT_RENDERER = &DOT_RENDERER_STRUCT;