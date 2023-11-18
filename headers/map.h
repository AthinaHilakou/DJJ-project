#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct { /* table entry: */
    struct map_node *next; /* next entry in chain */
    int key; /* defined key */
}map_node, *Map_node;


typedef struct {
    Map_node *array; /* array of pointers to entries */
    int size; /* size of array */
    int capacity; /* capacity of array */
}map, *Map;


// Functions
// Map Create
Map map_init(int capacity);

// Map Find
unsigned int hash(int s, int capacity);

void mapify(Map map, int * array, int size);

// Map Insert
bool map_update(Map map, int key, int key_remove);

// Map Remove
bool map_add(Map map, int key);

// Map Update
bool map_remove(Map map, int key);

// Map Destroy
void map_destroy(Map map);
