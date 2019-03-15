#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <argp.h> // This is awesome: http://nongnu.askapache.com/argpbook/step-by-step-into-argp.pdf
#include <regex.h>
#include "instr.h"
#include "list.h"
#include "hash.h"
#include "bblock.h"
#include "cfg_renderer.h"
#include "cfg_dot_renderer.h"

regex_t jumpRegex;
regex_t labelRegex;
regex_t isUnconditionalRegex;
regex_t isReturnRegex;

void compileRegexes(void) {
  if(regcomp(
    &jumpRegex,
    "[ \t]*[0-9a-f]+:[ \t]+j[a-z]+[ \t]+([0-9a-f]+)[ \t]+.*",
    REG_EXTENDED
  )) {
    printf("Fatal bug: jump regex did not compile.\n");
    exit(-2);
  }
  if (regcomp(
    &labelRegex,
    "[ \t]*([0-9a-f]+):[ \t]+[a-z]+[ \t]+.*",
    REG_EXTENDED
  )) {
    printf("Fatal bug: label regex did not compile.\n");
    exit(-2);
  }
  if (regcomp(
    &isUnconditionalRegex,
    "[ \t]*([0-9a-f]+):[ \t]+jmp[ \t]+.*",
    REG_EXTENDED
  )) {
    printf("Fatal bug: isUnconditionalRegex did not compile.\n");
    exit(-2);
  }
  if (regcomp(
    &isReturnRegex,
    "[ \t]*([0-9a-f]+):[ \t]+ret[ \t]+.*",
    REG_EXTENDED
  )) {
    printf("Fatal bug: isReturnRegex did not compile.\n");
    exit(-2);
  }
}

void commentLine(char* line) {
  const unsigned int NUM_GROUPS = 2;
  regmatch_t matches[NUM_GROUPS];
  if(regexec(&jumpRegex, line, NUM_GROUPS, matches, 0) == 0) {
    printf("############ jump MATCHED...\n");
    int g = 0;
    char temp = 0;
    for (g = 0; g < NUM_GROUPS; g++) {
      temp = line[matches[g].rm_eo];
      line[matches[g].rm_eo] = '\0';
      printf(
        "match %d of length %d: >>>%s<<<\n", 
        g,
        matches[g].rm_eo - matches[g].rm_so,
        &line[matches[g].rm_so]
      );
      line[matches[g].rm_eo] = temp;
    }
  } else {
    printf("---jump didn't match\n");
  }
}

instr parse_instr(char* line) {
  instr res = instr_new(line);
  const unsigned int MAX_GROUPS = 2;
  regmatch_t matches[MAX_GROUPS];
  if (regexec(&jumpRegex, line, MAX_GROUPS, matches, 0) == 0) {
    int JMP_GRP = 1;
    regmatch_t jmpMatch = matches[JMP_GRP];
    int len = jmpMatch.rm_eo - jmpMatch.rm_so;
    instr_set_jump(res, &line[jmpMatch.rm_so], len);
  }
  if (regexec(&labelRegex, line, MAX_GROUPS, matches, 0) == 0) {
    int LBL_GRP = 1;
    regmatch_t lblMatch = matches[LBL_GRP];
    int len = lblMatch.rm_eo - lblMatch.rm_so;
    instr_set_label(res, &line[lblMatch.rm_so], len);
  }
  if (regexec(&isReturnRegex, line, MAX_GROUPS, matches, 0) == 0) {
    res->isReturn = true;
  }
  if (regexec(&isUnconditionalRegex, line, MAX_GROUPS, matches, 0) == 0) {
    res->isUnconditionalJump = true;
  }
  return res;
}

list parse_input(FILE *inputFile) {
  list reversedAcc = list_empty();
  const int MAX_LINE_LENGTH=1024;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, MAX_LINE_LENGTH, inputFile)) {
    int len = strlen(line);
    if (len >= MAX_LINE_LENGTH) {
      printf("FATAL ERROR: input line too long. Emergency stop.\n");
    }
    line[len - 1] = '\0'; // cut away the line break
    instr i = parse_instr(line);
    reversedAcc = list_cons((void *) i, reversedAcc);
  }
  
  fclose(inputFile);

  list result = list_reverse(reversedAcc);
  list_free(reversedAcc);
  return result;
}



int main(int argc, char** args) {
  if (argc < 2) {
    printf("Expects file with assembly as single argument.");
    exit(1);
  }
  compileRegexes();
  FILE *inputFile = fopen(args[1], "r");
  list instructions = parse_input(inputFile);
  list basicBlocks = group_into_basic_blocks(instructions);
  cfg_renderer renderer = DOT_RENDERER;

  cfg_render(renderer, basicBlocks);
  
  /*
  printf("---- parsed instructions ----\n");
  foreach_list(instr, iter, instructions) {
    printf("%s    ", i->line);
    if (i->label) {
      printf("[###label = %s ###]", i->label);
    }
    if (i->jump) {
      printf("[$$$jump = %s $$$]", i->jump);
    }
    printf("\n");
  }
  printf("--- end of parsed instructions ---\n");


  printf("----- detected basic blocks -----\n");
  foreach_list(bblock, b, basicBlocks) {
    printf("bb-start: %s\n", bblock_getEntryLabel(b));
    foreach_list(instr, i, b->instructions) {
      printf("  %s\n", i->line);
    }
    printf("bb-end\n");
  }
  printf("-----/detected basic blocks -----\n");
  */


}

