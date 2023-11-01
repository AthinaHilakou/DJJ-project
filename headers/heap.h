//An implementation of an ADT heap
//heap stores indexes of data array 
#include <stdlib.h>
#include <stdbool.h>
#pragma once
//Structures 
typedef struct{
    int index; //index of item in data array 
    float weight; //dist of item from data_of_interest, a positive integer
}node ,*Node;

typedef struct{
    Node array; //array of indexes
    int size; //number of items in array
    int capacity; //heap capacity 
}heap, *Heap;


//Functions
/*Heap Create
  data_array: array of int indexes
  data_of_interest: index of item we want to calculate distances from 
  array_size: size of data_array
  capacity: heap capacity
  returns a max heap based on parameters specified
  */
Heap heap_create(int *data_array, int array_size, float *weights);
/*Inserts item in heap h, corresponding to index specified*/
void heap_insert(Heap h, int index, float weight);
/*Heap pop
  pops top item from heap h */
int heap_pop(Heap h);
/*Heap update 
  inserts index in heap provided that thw weight corresponding to index is smaller than that of the old root,
  If index is inserted the old root is popped and returned in old_root*/
bool heap_update(Heap h, int index, float weight);//upon return old_root contains the old root of the heap, if it was updated, else null
/*Heap Find Max 
  returns index of heap top*/
int heap_find_max(Heap h);
/*Heap Destroy
  deallocates memory for heap*/
int index_from_heap(Heap h, int node_index);

void print_heap(Heap h);

void heap_destroy(Heap h);


//Helper functions
void heapify(Heap h);
void bubble_down(Heap h, int root);
void bubble_up(Heap h, int child);
int get_heap_size(Heap h);
