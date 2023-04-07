CC=gcc
MKDIR=mkdir
MAKE=make
RM=rm -f
BIN:=bin

CFLAGS:=-g -O0 -Wall -std=c99 -fsanitize=address -fsanitize=undefined

ifdef release
CFLAGS:=-O2 -Wall -std=c99 -DNDEBUG
AOCAUX_FLAGS:=release=1
endif

ifndef verbose
SILENT=@
AOCAUX_FLAGS+=-s
else
AOCAUX_FLAGS+=verbose=1
endif

LIB_DIR:=aocaux
LIBS:=-lm -laocaux
LIBS_PATH:=-L$(LIB_DIR)/bin
LIBS_INCLUDE:=-I$(LIB_DIR)/src

aux: $(BIN)
	$(SILENT) $(MAKE) -C $(LIB_DIR) $(AOCUX_FLAGS)

day01: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day01 day01/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day02: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day02 day02/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day03: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day03 day03/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day04: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day04 day04/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day05: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day05 day05/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day06: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day06 day06/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day07: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day07 day07/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day08: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day08 day08/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day09: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day09 day09/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day10: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day10 day10/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day11: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day11 day11/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day12: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day12 day12/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day13: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day13 day13/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day14: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day14 day14/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day15: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day15 day15/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day16: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day16 day16/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day17: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day17 day17/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day18: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day18 day18/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day19: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day19 day19/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day20: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day20 day20/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day21: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day21 day21/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day22: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day22 day22/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day23: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day23 day23/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day24: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day24 day24/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

day25: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day25 day25/main.c $(LIBS_INCLUDE) $(LIBS_PATH) $(LIBS)

$(BIN)/%.o: aux/%.c
	$(SILENT) $(CC) -c $(CFLAGS) $< -o $@

$(BIN):
	$(SILENT) $(MKDIR) -p $(BIN)

clean:
	$(SILENT) $(RM) $(BIN)/*
	$(SILENT) $(MAKE) clean -C $(LIB_DIR) $(AOCUX_FLAGS)

.PHONY: aux day01 day02 day03 day04 day05 \
  day06 day07 day08 day09 day10 day11 day12 \
  day13 day14 day15 day16 day17 day18 day19 \
  day20 day21 day22 day23 day24 day25 clean 
