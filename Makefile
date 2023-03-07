CC:=gcc
CFLAGS:=-g -O0 -Wall -std=c99 -fsanitize=address -fsanitize=undefined
BIN:=bin

AUX_SOURCES:=$(wildcard aux/*.c)
AUX_OBJS:=$(patsubst aux/%.c, $(BIN)/%.o, $(AUX_SOURCES))

aux: $(BIN) $(AUX_OBJS)
	ar rcs $(BIN)/libaux.a $(AUX_OBJS)

day01: $(BIN) aux
	$(CC) $(CFLAGS) -o $(BIN)/day01 day01/main.c -Iaux -L$(BIN) -laux -lm

$(BIN)/%.o: aux/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN):
	mkdir -p $(BIN)

clean:
	rm -rf ./$(BIN)/*

.PHONY: aux day01 clean