CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/messenger.c ./src/main.c
OBJECTS=$(SOURCES:.c=.o)
BIN_PATH=./bin
EXECUTABLE=$(BIN_PATH)/messenger

all: clean messenger

messenger:
	$(CC) $(CFLAGS1) -g $(SOURCES) -o $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/*

rebuild:
	make clean
	make all
