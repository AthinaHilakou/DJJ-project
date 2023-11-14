CC = gcc
CFLAGS = #-Wall -Wextra -pedantic -std=c99
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TESTS_DIR = tests
HEADERS = headers

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
BIN = $(BIN_DIR)/main
TESTS = $(wildcard $(TESTS_DIR)/*.c)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $^ -o $@ -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: $(BIN)
	./$(BIN)

test_heap.o: tests/test_heap.c src/heap.c
	gcc -c tests/test_heap.c -o tests/test_heap.o
	gcc -c src/heap.c -o obj/heap.o -I$(HEADERS)


test_heap: tests/test_heap.o obj/heap.o
	gcc -o tests/test_heap tests/test_heap.o obj/heap.o -lm
	./tests/test_heap

# test_min_heap.o: tests/test_min_heap.c src/min_heap.c
# 	gcc -c tests/test_min_heap.c -o tests/test_min_heap.o -I$(HEADERS)
# 	gcc -c src/min_heap.c -o obj/min_heap.o -I$(HEADERS)

test_min_heap: tests/test_min_heap.o obj/min_heap.o
	gcc -o tests/test_min_heap tests/test_min_heap.o obj/min_heap.o -lm
	./tests/test_min_heap



test_min_heap.o: tests/test_min_heap.c src/min_heap.c
	gcc -c tests/test_min_heap.c -o tests/test_min_heap.o -I$(HEADERS)


tests: $(TESTS) $(BIN)
	gcc  -o test_of_heap tests/test_heap.c obj/heap.o -lm
	gcc  -o test_of_min_heap tests/test_min_heap.c obj/min_heap.o -lm
	./test_of_heap
	./test_of_min_heap


# $(CC) $(CFLAGS) -I$(SRC_DIR) -o test_of_heap tests/test_heap.c $(OBJS) -lm
# $(CC) $(CFLAGS) -I$(SRC_DIR) obj/heap.o obj/min_heap.o  $(TESTS) -lm
# ./$(TESTS:.c=)
do:
	./$(TESTS:.c=)

clean:
	rm -rf $(OBJ_DIR) $(BIN)
