CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=calc.c user_interface.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=calc

all: clean calc

calc:
	$(CC) $(CFLAGS1) $(SOURCES) -o $(EXECUTABLE)
	./$(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

rebuild:
	make clean
	make all
