CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=mask.c user_interface.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mask

all: clean mask

mask:
	$(CC) $(CFLAGS1) $(SOURCES) -o $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

rebuild:
	make clean
	make all
