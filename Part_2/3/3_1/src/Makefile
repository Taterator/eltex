CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=chmod.c user_interface.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=chmod

all: clean chmod

chmod:
	$(CC) $(CFLAGS1) $(SOURCES) -o $(EXECUTABLE)
	./$(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

rebuild:
	make clean
	make all
