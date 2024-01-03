CC = gcc
CFLAGS = #-Wall -Wextra -pedantic -std=c99
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TESTS_DIR = tests
HEADERS = headers
# max_neighbors/k filename manh/eucl data_type_flag delta sampling_rate
# ARGS = 100 datasets/given/00005000-2.bin eucl 0 0.01 0.08
ARGS = 4 datasets/given/00000020.bin eucl_opt 0 0.001 1

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
	

run_projection: src/RAND_PROJ_T.c
	gcc -o obj/projection src/RAND_PROJ_T.c -lm
	./obj/projection

test_heap: tests/test_heap.o obj/heap.o
	gcc -o obj/test_heap tests/test_heap.o obj/heap.o -lm
	./obj/test_heap

test_graph: tests/test_graph.c obj/graph.o
	gcc -c src/graph.c -o obj/graph.o -I$(HEADERS)
	gcc -c tests/test_graph.c -o obj/test_graph.o -I$(HEADERS)
	gcc -o obj/test_graph obj/test_graph.o obj/graph.o -lm
	./obj/test_graph

test_min_heap: tests/test_min_heap.o obj/min_heap.o
	gcc -o obj/test_min_heap tests/test_min_heap.o obj/min_heap.o -lm
	./obj/test_min_heap


test_avl: tests/test_avl_tree.c src/avl_tree.c
	gcc -c src/avl_tree.c -o obj/avl_tree.o -I$(HEADERS)
	gcc -c tests/test_avl.c -o obj/test_avl_tree.o -I$(HEADERS)
	gcc -o obj/test_avl obj/test_avl_tree.o obj/avl_tree.o -lm
	./obj/test_avl

tests: $(TESTS) $(BIN)
	gcc  -o obj/test_of_heap tests/test_heap.c obj/heap.o -lm
	gcc  -o obj/test_of_min_heap tests/test_min_heap.c obj/min_heap.o -lm
	gcc  -o obj/test_graph tests/test_graph.c obj/graph.o -lm
	gcc  -o obj/test_avl tests/test_avl.c obj/avl_tree.o -lm
	./obj/test_graph 
	./obj/test_of_heap
	./obj/test_of_min_heap
	./obj/test_avl

do:
	./obj/test_graph 
	./obj/test_of_heap
	./obj/test_of_min_heap

clean:
	rm -rf $(OBJS) $(BIN) $(BIN_DIR)/real_neighbors.txt obj/test_graph obj/test_of_heap obj/test_of_min_heap obj/test_avl
