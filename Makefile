CC:=gcc
CFLAGS:=-g -O0 -Wall -std=c99 -fsanitize=address -fsanitize=undefined
# CFLAGS:=-O3 -Wall -std=c99 -DNDEBUG
BIN:=bin

AUX_SOURCES:=$(wildcard aux/*.c)
AUX_OBJS:=$(patsubst aux/%.c, $(BIN)/%.o, $(AUX_SOURCES))

ifndef verbose
	SILENT=@
endif

all: $(BIN) aux day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 day11 day12 day13

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

$(BIN)/%.o: aux/%.c
	$(SILENT) $(CC) -c $(CFLAGS) $< -o $@

$(BIN):
	$(SILENT) mkdir -p $(BIN)

clean:
	$(SILENT) rm -rf ./$(BIN)/*

.PHONY: all aux day01 day02 day03 day04 day05 day06 day07 day08 day09 day10 day11 day12 day13 clean
