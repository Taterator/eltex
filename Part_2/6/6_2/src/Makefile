CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
LIB_SOURCES=book.c
SOURCES=user_interface.c
LIB_OBJECTS=$(LIB_SOURCES:.c=.o)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=book

all: clean book

book: back.a
	$(CC) $(CFLAGS1) -g $(SOURCES) -L./ -lback -o $(EXECUTABLE)
	./$(EXECUTABLE)

back.a:
	$(CC) -fPIC $(CFLAGS) $(LIB_SOURCES)
	$(CC) -shared $(LIB_OBJECTS) -o libback.so

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(LIB_OBJECTS) libback.so

rebuild:
	make clean
	make all
