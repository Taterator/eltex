CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/common.c
OBJECTS=$(SOURCES:.c=.o)
BIN_PATH=./bin
SRC_PATH=./src
EXECUTABLE=$(BIN_PATH)/messenger

all: clean printer reader

printer:
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

reader:
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

cleaner:
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/*

rebuild:
	make clean
	make all
