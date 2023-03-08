CC:=gcc
CFLAGS:=-g -O0 -Wall -std=c99 -fsanitize=address -fsanitize=undefined
BIN:=bin

AUX_SOURCES:=$(wildcard aux/*.c)
AUX_OBJS:=$(patsubst aux/%.c, $(BIN)/%.o, $(AUX_SOURCES))

ifndef verbose
	SILENT=@
endif

aux: $(BIN) $(AUX_OBJS)
	$(SILENT) ar rcs $(BIN)/libaux.a $(AUX_OBJS)

day01: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day01 day01/main.c -Iaux -L$(BIN) -laux -lm

day02: $(BIN) aux
	$(SILENT) $(CC) $(CFLAGS) -o $(BIN)/day02 day02/main.c -Iaux -L$(BIN) -laux -lm

$(BIN)/%.o: aux/%.c
	$(SILENT) $(CC) -c $(CFLAGS) $< -o $@

$(BIN):
	$(SILENT) mkdir -p $(BIN)

clean:
	$(SILENT) rm -rf ./$(BIN)/*

.PHONY: aux day01 day02 clean
