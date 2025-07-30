CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/interpreter.c ./src/user_interface.c
OBJECTS=$(SOURCES:.c=.o)
BIN_PATH=./bin
EXECUTABLE=$(BIN_PATH)/interpreter
PROG_PATH=./src/programs

all: clean interpreter

interpreter:
	$(CC) $(CFLAGS1) -g $(SOURCES) -o $(EXECUTABLE)
	$(EXECUTABLE)

progs: sum max

sum:
	$(CC) $(CFLAGS1) -g $(PROG_PATH)/$@.c -o $(BIN_PATH)/$@

max:
	$(CC) $(CFLAGS1) -g $(PROG_PATH)/$@.c -o $(BIN_PATH)/$@

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/*

rebuild:
	make clean
	make all
