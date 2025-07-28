CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
LIB_SOURCES=add.c sub.c div.c mul.c
LIBS=libadd.so libsub.so libdiv.so libmul.so
SOURCES=user_interface.c
LIB_OBJECTS=$(LIB_SOURCES:.c=.o)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=calc
#LD_LIBRARY_PATH=./:$(LD_LIBRARY_PATH)

all: clean calc

calc: $(LIBS)
	$(CC) $(CFLAGS1) -rdynamic $(SOURCES) -L./ -ladd -lsub -ldiv -lmul -ldl -o $(EXECUTABLE)
	./$(EXECUTABLE)

all_libs: $(LIBS)

libadd.so: add.c
	$(CC) -fPIC $(CFLAGS) add.c
	$(CC) -shared add.o -o libadd.so

libsub.so: sub.c
	$(CC) -fPIC $(CFLAGS) sub.c
	$(CC) -shared sub.o -o libsub.so

libdiv.so: div.c
	$(CC) -fPIC $(CFLAGS) div.c
	$(CC) -shared div.o -o libdiv.so

libmul.so: mul.c
	$(CC) -fPIC $(CFLAGS) mul.c
	$(CC) -shared mul.o -o libmul.so

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(LIBS) $(LIB_OBJECTS)

rebuild:
	make clean
	make all
