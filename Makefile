CC = gcc
TARGETS = engine.c
BIN = engine
LIBS = -lncurses -lm
FLAGS = -Wall


program engine:
	$(CC) $(TARGETS) -o $(BIN) $(LIBS) $(FLAGS)
