CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/common.c
OBJECTS=$(SOURCES:.c=.o)
BIN_PATH=./bin
SRC_PATH=./src
EXECUTABLE=$(BIN_PATH)/messenger

all: clean server client

server:
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c $(SOURCES) -o $(BIN_PATH)/$@

client:
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c $(SOURCES) -o $(BIN_PATH)/$@

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/*

rebuild:
	make clean
	make all
