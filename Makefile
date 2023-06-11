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

SOURCES:=$(wildcard */main.c)

LIB_DIR:=aocaux
LIBS_PATH:=$(LIB_DIR)/bin
LIBS_INCLUDE:=$(LIB_DIR)/src

vpath %.a $(LIBS_PATH)

-laocaux: | $(BIN)
	$(SILENT) $(MAKE) -C $(LIB_DIR) $(AOCAUX_FLAGS)

$(BIN)/day%: -laocaux $(SOURCES) | $(BIN) 
	$(SILENT) $(CC) $(CFLAGS) -o $@ $(subst $(BIN)/,,$@)/main.c -I$(LIBS_INCLUDE) -L$(LIBS_PATH) $< -lm

$(BIN):
	$(SILENT) $(MKDIR) -p $(BIN)

clean:
	$(SILENT) $(RM) $(BIN)/*
	$(SILENT) $(MAKE) clean -C $(LIB_DIR) $(AOCAUX_FLAGS)

.PHONY: clean 
