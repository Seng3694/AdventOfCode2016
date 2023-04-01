CC:=gcc
CFLAGS:=-g -O0 -Wall -std=c99 -fsanitize=address -fsanitize=undefined
# CFLAGS:=-O3 -Wall -std=c99 -DNDEBUG
BIN:=bin

AUX_SOURCES:=$(wildcard aux/*.c)
AUX_OBJS:=$(patsubst aux/%.c, $(BIN)/%.o, $(AUX_SOURCES))

ifndef verbose
	SILENT=@
endif

profiling: CFLAGS=-g -pg -O2 -Wall -std=c99
profiling: day14

all: $(BIN) aux day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 day21 day22 day23

aux: $(BIN) $(AUX_OBJS)
	$(SILENT) ar rcs $(BIN)/libaux.a $(AUX_OBJS)

day01: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day01 day01/main.c -Iaux -L$(BIN) -laux -lm

day02: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day02 day02/main.c -Iaux -L$(BIN) -laux -lm

day03: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day03 day03/main.c -Iaux -L$(BIN) -laux -lm

day04: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day04 day04/main.c -Iaux -L$(BIN) -laux -lm

day05: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day05 day05/main.c -Iaux -L$(BIN) -laux -lm

day06: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day06 day06/main.c -Iaux -L$(BIN) -laux -lm

day07: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day07 day07/main.c -Iaux -L$(BIN) -laux -lm

day08: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day08 day08/main.c -Iaux -L$(BIN) -laux -lm

day09: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day09 day09/main.c -Iaux -L$(BIN) -laux -lm

day10: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day10 day10/main.c -Iaux -L$(BIN) -laux -lm

day11: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day11 day11/main.c -Iaux -L$(BIN) -laux -lm

day12: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day12 day12/main.c -Iaux -L$(BIN) -laux -lm

day13: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day13 day13/main.c -Iaux -L$(BIN) -laux -lm

day14: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day14 day14/main.c -Iaux -L$(BIN) -laux -lm

day15: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day15 day15/main.c -Iaux -L$(BIN) -laux -lm

day16: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day16 day16/main.c -Iaux -L$(BIN) -laux -lm

day17: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day17 day17/main.c -Iaux -L$(BIN) -laux -lm

day18: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day18 day18/main.c -Iaux -L$(BIN) -laux -lm

day19: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day19 day19/main.c -Iaux -L$(BIN) -laux -lm

day20: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day20 day20/main.c -Iaux -L$(BIN) -laux -lm

day21: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day21 day21/main.c -Iaux -L$(BIN) -laux -lm

day22: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day22 day22/main.c -Iaux -L$(BIN) -laux -lm

day23: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day23 day23/main.c -Iaux -L$(BIN) -laux -lm

$(BIN)/%.o: aux/%.c
	$(SILENT) $(CC) -c $(CFLAGS) $< -o $@

$(BIN):
	$(SILENT) mkdir -p $(BIN)

clean:
	$(SILENT) rm -rf ./$(BIN)/*

.PHONY: all aux day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 day21 day22 day23 clean profiling
