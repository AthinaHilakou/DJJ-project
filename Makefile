CC = gcc
CFLAGS = #-Wall -Wextra -pedantic -std=c99
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TESTS_DIR = tests
HEADERS = headers
# max_neighbors/k filename manh/eucl data_type_flag delta sampling_rate
# ARGS = 100 datasets/given/00050000-2.bin eucl 0 0.1 0.02
ARGS = 40 datasets/given/00002000-2.bin eucl_opt 0 0.001 0.8 1

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
	
test_heap: tests/test_heap.c src/heap.c src/data.c
	gcc -c src/data.c -o obj/data.o
	gcc -c src/heap.c -o obj/heap.o
	gcc -c tests/test_heap.c -o obj/test_heap.o
	gcc -o bin/test_heap obj/test_heap.o obj/heap.o obj/data.o -lm
	./bin/test_heap

test_graph: tests/test_graph.c src/graph.c
	gcc -c src/graph.c -o obj/graph.o
	gcc -c tests/test_graph.c -o obj/test_graph.o
	gcc -o bin/test_graph obj/test_graph.o obj/graph.o -lm
	./bin/test_graph

test_min_heap: tests/test_min_heap.c src/min_heap.c
	gcc -c src/min_heap.c -o obj/min_heap.o
	gcc -c tests/test_min_heap.c -o obj/test_min_heap.o
	gcc -o bin/test_min_heap obj/test_min_heap.o obj/min_heap.o -lm
	./bin/test_min_heap


test_avl: tests/test_avl.c src/avl_tree.c
	gcc -c src/avl_tree.c -o obj/avl_tree.o
	gcc -c tests/test_avl.c -o obj/test_avl.o
	gcc -o bin/test_avl obj/test_avl.o obj/avl_tree.o -lm
	./bin/test_avl

test_rpt: tests/test_rpt.c src/rpt.c
	gcc -c src/rpt.c -o obj/rpt.o
	gcc -c tests/test_rpt.c -o obj/test_rpt.o
	gcc -o bin/test_rpt obj/test_rpt.o obj/rpt.o -lm
	./bin/test_rpt

tests: $(TESTS) $(BIN)
	gcc  -o bin/test_heap tests/test_heap.c obj/heap.o -lm
	gcc  -o bin/test_min_heap tests/test_min_heap.c obj/min_heap.o -lm
	gcc  -o bin/test_graph tests/test_graph.c obj/graph.o -lm
	gcc  -o bin/test_avl tests/test_avl.c obj/avl_tree.o -lm
	./bin/test_graph 
	./bin/test_heap
	./bin/test_min_heap
	./bin/test_avl

do:
	./bin/test_graph 
	./bin/test_heap
	./bin/test_min_heap
	./bin/test_avl

clean:
	rm -rf $(OBJS) $(BIN) $(BIN_DIR)/real_neighbors.txt bin/test_graph bin/test_of_heap bin/test_of_min_heap bin/test_avl bin/min_heap bin/test_min_heap bin/test_heap obj/test_graph.o obj/test_heap.o obj/test_min_heap.o obj/test_avl.o
