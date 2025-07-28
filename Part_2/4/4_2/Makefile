CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11
CFLAGS1= -Wall -Werror -Wextra -std=c11
LDFLAGS=
SOURCES=./src/queue.c ./src/user_interface.c
TEST_SOURCES=./src/queue.c ./tests/test_queue.c ./unity/unity.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=./bin/queue
TEST_EXECUTABLE=./bin/test

all: clean queue

queue:
	$(CC) $(CFLAGS1) -g $(SOURCES) -o $(EXECUTABLE)
	$(EXECUTABLE)

test:
	$(CC) $(CFLAGS1) -g $(TEST_SOURCES) -o $(TEST_EXECUTABLE)
	$(TEST_EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(TEST_EXECUTABLE)

rebuild:
	make clean
	make all
