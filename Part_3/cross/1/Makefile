CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/server.c ./src/client.c
COMMON=./src/udp_msg.c
OBJECTS=$(SOURCES:.c=.o)
BIN_PATH=./bin
SRC_PATH=./src
EXECUTABLE1=$(BIN_PATH)/server
EXECUTABLE2=$(BIN_PATH)/client

all: clean server client

server:
	mkdir -p $(BIN_PATH)
	$(CC) $(CFLAGS1) -g $(COMMON) $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

client:
	mkdir -p $(BIN_PATH)
	$(CC) $(CFLAGS1) -g $(COMMON) $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/* 

rebuild:
	make clean
	make all
