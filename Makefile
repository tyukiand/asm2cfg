# directories with sources and targets
MSRC=src/main/c
TSRC=src/test/c
MTRG=trg
TTRG=trg/test

OBJECTS=$(MTRG)/hash.o $(MTRG)/list.o $(MTRG)/instr.o $(MTRG)/bblock.o $(MTRG)/cfg_renderer.o $(MTRG)/cfg_dot_renderer.o
CFLAGS= -g -Wall -O3 -D_GNU_SOURCE
LDLIBS=
CC=gcc
PROG=asm2cfg
COLL=$$(pkg-config --cflags)

.PHONY: clean

$(MTRG)/$(PROG): $(MTRG) $(OBJECTS)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(MTRG)/$(PROG) $(MSRC)/$(PROG).c \
	$(OBJECTS) \
	$(COLL)

$(MTRG)/hash.o: $(MTRG) $(MSRC)/hash.c $(MSRC)/hash.h
	gcc -std=c11 -c -g -Wall -O3 -o $(MTRG)/hash.o $(MSRC)/hash.c

$(MTRG)/list.o: $(MTRG) $(MSRC)/list.c $(MSRC)/list.h
	gcc -std=c11 -c -g -Wall -O3 -o $(MTRG)/list.o $(MSRC)/list.c

$(MTRG)/instr.o: $(MTRG) $(MSRC)/instr.h $(MSRC)/instr.c
	$(CC) $(CFLAGS) -c -o $(MTRG)/instr.o $(MSRC)/instr.c

$(MTRG)/bblock.o: $(MTRG) $(MSRC)/bblock.h $(MSRC)/bblock.c $(MSRC)/instr.h
	$(CC) $(CFLAGS) -c $(MSRC)/bblock.c $(COLL) -o $(MTRG)/bblock.o

$(MTRG)/cfg_dot_renderer.o: $(MTRG) $(MSRC)/cfg_dot_renderer.h $(MSRC)/cfg_dot_renderer.c
	$(CC) $(CFLAGS) -c $(MSRC)/cfg_dot_renderer.c $(COLL) -o $(MTRG)/cfg_dot_renderer.o

$(MTRG)/cfg_renderer.o: $(MTRG) $(MSRC)/cfg_renderer.h $(MSRC)/cfg_renderer.c
	$(CC) $(CFLAGS) -c $(MSRC)/cfg_renderer.c $(COLL) -o $(MTRG)/cfg_renderer.o


clean:
	rm -r ./$(MTRG)

$(MTRG):
	mkdir -p $(MTRG)

$(TTRG):
	mkdir -p $(TTRG)
