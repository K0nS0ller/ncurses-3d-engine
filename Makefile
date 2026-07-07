CC = gcc
TARGETS = main.c
BIN = engine
LIBS = -lncurses -lm
FLAGS = -Wall


program engine:
	$(CC) $(TARGETS) -o $(BIN) $(LIBS) $(FLAGS)
