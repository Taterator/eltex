CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/sniffer.c
OBJECTS=$(SOURCES:.c=.o)
BIN_PATH=./bin
SRC_PATH=./src
EXECUTABLE=$(BIN_PATH)/sniffer

all: clean sniffer

sniffer:
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/* dump.bin dump2.bin

rebuild:
	make clean
	make all
