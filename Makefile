CC = gcc
TARGET = engine.c
BIN = engine
LIBS = -lncurses -lm
FLAGS = -Wall

.PHONY: all clean

all: $(BIN)

$(BIN): $(TARGET)
	$(CC) $(FLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(BIN)
