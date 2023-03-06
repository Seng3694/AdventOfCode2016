CC:=gcc
CFLAGS:=-g -O0 -Wall -std=c99 -fsanitize=address -fsanitize=undefined
BIN:=bin

day01:
	mkdir -p $(BIN) 
	$(CC) $(CFLAGS) -o $(BIN)/day01 day01/main.c -lm

clean:
	rm -rf ./$(BIN)/*

.PHONY: day01 clean