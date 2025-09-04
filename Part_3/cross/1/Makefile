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
	$(CC) $(CFLAGS1) -g $(COMMON) $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

client:
	$(CC) $(CFLAGS1) -g $(COMMON) $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

udp_test:
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

tcp_test:
	$(CC) $(CFLAGS1) -g $(COMMON) $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

run1:
	./$(EXECUTABLE1) 127.0.0.23 55555

run2:
	./$(EXECUTABLE2) 127.0.0.24 44444 127.0.0.23 55555

run3:
	./bin/udp_test 127.0.0.24 44444 127.0.0.23 55555

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/* dump.bin

rebuild:
	make clean
	make all
