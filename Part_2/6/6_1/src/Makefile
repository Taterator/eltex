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
	$(CC) $(CFLAGS1) -g $(SOURCES) -L . -lback -o $(EXECUTABLE)
	./$(EXECUTABLE)

back.a:
	$(CC) $(CFLAGS) $(LIB_SOURCES)
	ar rc libback.a $(LIB_OBJECTS)
	ranlib libback.a

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(LIB_OBJECTS) libback.a

rebuild:
	make clean
	make all
