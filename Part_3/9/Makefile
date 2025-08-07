CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/main.c ./src/reader.c ./src/printer.c
OBJECTS=$(SOURCES:.c=.o)
BIN_PATH=./bin
SRC_PATH=./src
EXECUTABLE=$(BIN_PATH)/messenger

all: clean semaphore

semaphore:
	$(CC) $(CFLAGS1) -g $(SOURCES) -o $(BIN_PATH)/$@


.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/* text_file

rebuild:
	make clean
	make all
