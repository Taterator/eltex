CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
BIN_PATH=./bin
SRC_PATH=./src

all: clean task

task: server client

server:
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

client: 
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(OBJECTS) $(BIN_PATH)/*

rebuild:
	make clean
	make all
