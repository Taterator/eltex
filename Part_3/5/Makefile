CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/signal_timer.c ./src/user_interface.c
OBJECTS=$(SOURCES:.c=.o)
BIN_PATH=./bin
EXECUTABLE=$(BIN_PATH)/signal_timer

all: signal_timer

signal_timer:
	$(CC) $(CFLAGS1) -g $(SOURCES) -o $(EXECUTABLE)

run: 
	$(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BIN_PATH)/* output_file.txt

rebuild:
	make clean
	make all
