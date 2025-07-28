CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/arg_parser.c ./src/user_interface.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=./bin/arg_parser

all: clean arg_parser

arg_parser:
	$(CC) $(CFLAGS1) -g $(SOURCES) -o $(EXECUTABLE)
	$(EXECUTABLE)

test:
	$(CC) $(CFLAGS1) -g $(SOURCES) -o $(EXECUTABLE)
	$(EXECUTABLE) a b lsdasda 23 1.234 1.33.4 .656 dsa potato 2.43 @fdsfds #$#@#$gsdf

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

rebuild:
	make clean
	make all
