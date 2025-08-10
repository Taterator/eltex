CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/messenger.c
OBJECTS=$(SOURCES:.c=.o)
BIN_PATH=./bin
SRC_PATH=./src
EXECUTABLE=$(BIN_PATH)/messenger

all: clean messenger

messenger:
	$(CC) $(CFLAGS1) -g $(SRC_PATH)/$@.c -o $(BIN_PATH)/$@

run1:
	./$(EXECUTABLE) 127.0.0.23 55555 127.0.0.24 44444

run2:
	./$(EXECUTABLE) 127.0.0.24 44444 127.0.0.23 55555

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/*

rebuild:
	make clean
	make all
