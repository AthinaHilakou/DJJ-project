CC = gcc
CFLAGS = #-Wall -Wextra -pedantic -std=c99
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TESTS_DIR = tests
HEADERS = headers
# max_neighbors/k filename manh/eucl data_type_flag delta sampling_rate
ARGS = 200 datasets/given/00001000-1.bin manh 0 0.01 0.4

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
	./$(BIN) $(ARGS)

runs: $(BIN)
	./$(BIN) $(ARGS)

test_heap: tests/test_heap.o obj/heap.o
	gcc -o tests/test_heap tests/test_heap.o obj/heap.o -lm
	./tests/test_heap

test_graph: tests/test_graph.c obj/graph.o
	gcc -c src/graph.c -o obj/graph.o -I$(HEADERS)
	gcc -c tests/test_graph.c -o obj/test_graph.o -I$(HEADERS)
	gcc -o tests/test_graph obj/test_graph.o obj/graph.o -lm
	./tests/test_graph

test_min_heap: tests/test_min_heap.o obj/min_heap.o
	gcc -o tests/test_min_heap tests/test_min_heap.o obj/min_heap.o -lm
	./tests/test_min_heap


test_map: tests/test_map.c src/map.c
	gcc -c src/map.c -o obj/map.o -I$(HEADERS)
	gcc -c tests/test_map.c -o obj/test_map.o -I$(HEADERS)
	gcc -o tests/test_map obj/test_map.o obj/map.o -lm
	./tests/test_map

tests: $(TESTS) $(BIN)
	gcc  -o tests/test_of_heap tests/test_heap.c obj/heap.o -lm
	gcc  -o tests/test_of_min_heap tests/test_min_heap.c obj/min_heap.o -lm
	gcc  -o tests/test_map tests/test_map.c obj/map.o -lm
	gcc  -o tests/test_graph tests/test_graph.c obj/graph.o -lm
	./tests/test_graph 
	./tests/test_of_heap
	./tests/test_of_min_heap
	# ./tests/test_map

do:
	./$(TESTS:.c=)

clean:
	rm -rf $(OBJS) $(BIN)
