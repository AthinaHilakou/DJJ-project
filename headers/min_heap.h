//An implementation of an ADT heap
//heap stores indexes of data array 
#include <stdlib.h>
#include <stdbool.h>
#pragma once
//Structures 
typedef struct{
    int index; //index of item in data array 
    float weight; //dist of item from data_of_interest, a positive integer
}min_node ,*min_Node;

typedef struct{
    min_Node array; //array of indexes
    int size; //number of items in array
    int capacity; //heap capacity 
}min_heap, *min_Heap;


//Functions
/*Heap Create
  data_array: array of int indexes
  data_of_interest: index of item we want to calculate distances from 
  array_size: size of data_array
  capacity: heap capacity
  returns a max heap based on parameters specified
  */
min_Heap min_heap_create(int *data_array, int array_size, float *weights);
/*Inserts item in heap h, corresponding to index specified*/
void min_heap_insert(min_Heap h, int index, float weight);
/*Heap pop
  pops top item from heap h */
int min_heap_pop(min_Heap h);
/*Heap update 
  inserts index in heap provided that thw weight corresponding to index is smaller than that of the old root,
  If index is inserted the old root is popped and returned in old_root*/
bool min_heap_update(min_Heap h, int index, float weight);//upon return old_root contains the old root of the heap, if it was updated, else null
/*Heap Find Max 
  returns index of heap top*/
int heap_find_min(min_Heap h);
/*Heap Destroy
  deallocates memory for heap*/
int index_from_min_heap(min_Heap h, int node_index);

void print_min_heap(min_Heap h);

void min_heap_destroy(min_Heap h);


//Helper functions
void min_heapify(min_Heap h);
void min_bubble_down(min_Heap h, int root);
void min_bubble_up(min_Heap h, int child);
int  min_get_heap_size(min_Heap h);
