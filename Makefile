CC = gcc
CFLAGS = #-Wall -Wextra -pedantic -std=c99
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TESTS_DIR = tests

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

tests: $(TESTS) $(BIN)
	touch $(TESTS)
	$(CC) $(CFLAGS) obj/heap.o $(TESTS_DIR)/test_heap.c -o $(TESTS_DIR)/test_heap -lm
	./$(TESTS_DIR)/test_heap
	$(CC) $(CFLAGS) obj/data.o $(TESTS_DIR)/test_import.c -o $(TESTS_DIR)/test_import -lm
	./$(TESTS_DIR)/test_import
do:
	./$(TESTS:.c=)

clean:
	rm -rf $(OBJ_DIR) $(BIN)
