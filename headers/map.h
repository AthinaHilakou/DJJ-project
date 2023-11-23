#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct my_map_node{ /* table entry: */
    struct my_map_node *next; /* next entry in chain */
    int key; /* defined key */
    float weight; /* defined weight */
   // int count; /* number of times key has been added */
}map_node, *Map_node;


typedef struct mymap{
    Map_node *array; /* array of pointers to entries */
    int size; /* size of array */
    int capacity; /* capacity of array */
}map, *Map;


// Functions

// Map Create
Map map_init(int capacity);

// Map hash function
unsigned int hash(int s, int capacity);

// Map Size
int get_map_size(Map map);


// Mapify an integer array
void mapify(Map map, int* array, float* weights, int size);

// Map Insert
bool map_update(Map map, int key, float weight, int key_remove);

// Map Add
bool map_add(Map map, int key, float weight);

// Map Update
bool map_remove(Map map, int key);

// Map Destroy
void map_destroy(Map map);

// Map Rehash
void map_rehash(Map map);

// Map Get
float map_get(Map map, int key);

// demapify
int *map_to_array(Map map, int* size);

// print
void map_print(Map map);
