#include "../headers/min_heap.h"
#include "../headers/data.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

min_Heap min_heap_create(int *data_array, int array_size, float *weights){
    min_Heap h = (min_Heap)malloc(sizeof(min_heap));
    // Checking if memory is allocated
    if (h == NULL) {
        printf("Memory error");
        return NULL;
    }
    //set the value of size
    h->size = array_size;   // size of input array
    h->capacity = 3*array_size/2 + 10; // current max size of heap
    //Allocating memory for items
    h->array = (min_Node)malloc((h->capacity)*sizeof(min_node));

    //Checking if memory is allocated
    if (h->array == NULL){
        printf("Memory error");
        return NULL;
    }

    //Put items in heap
    for(int i = 0; i < array_size; i++){
        h->array[i].index = data_array[i];
        // printf("\nindex: %d\n", h->array[i].index);
        h->array[i].weight = weights[i];
        // printf("weight: %d\n", h->array[i].weight);
    }

    //Heapify
    min_heapify(h);
    return h;
}

void min_heap_insert(min_Heap h, int index, float weight){
   
   if(h->size < h->capacity){
        h->array[h->size].index = index;  // insert item
        h->array[h->size].weight = weight; // insert weight
        h->size++;                  // update size of heap
        min_bubble_up(h,(h->size-1));   // restore heap property
   }
   else if(h->size == h->capacity){
        int new_capacity = (h->capacity)*2; // double capacity
        h->array = (min_Node)realloc(h->array, new_capacity*sizeof(min_node));  // reallocate memory
        h->capacity = new_capacity; // update capacity
        h->array[h->size].index = index; // insert item
        h->array[h->size].weight = weight; // insert weight
        h->size++;  // update size of heap
        min_bubble_up(h,(h->size-1)); // restore heap property
   }
}

int min_heap_pop(min_Heap h){
    if(h->size == 0)
        return -1;
    else if(h->size*4 == h->capacity){
        int new_capacity = h->capacity/2;
        h->array = (min_Node)realloc(h->array, new_capacity*sizeof(min_node));
        h->capacity = new_capacity;
    }
    int ret;
    ret = h->array[0].index;
    h->array[0].index = h->array[(h->size-1)].index;
    h->array[0].weight = h->array[(h->size-1)].weight;
    h->size--;
    min_bubble_down(h,0);
    return ret;
}

int get_min_heap_size(min_Heap h){
    return h->size;
}

int index_from_min_heap(min_Heap h, int node_index){
    return h->array[node_index].index;
}

bool min_heap_update(min_Heap h, int index, float weight){
    //printf("index = %d, weight = %f\n", index, weight);
    //printf("h->array[0].index = %d, h->array[0].weight = %f\n", h->array[0].index, h->array[0].weight);
   bool ret_value = false;  
   if(weight > h->array[0].weight){
        h->array[0].index = index;
        h->array[0].weight = weight;
        ret_value = true;
        min_bubble_down(h, 0); //bubble down from root to maintain heap property
   }
   return ret_value;
}

int heap_find_min(min_Heap h){
    return h->array[0].index;
}

void print_min_heap(min_Heap h){
    printf("Heap start \n");
    for(int i = 0; i < h->size; i++){
        printf("%d index: %d, weight: %f\n", i, h->array[i].index, h->array[i].weight);
    }
    printf("Heap end \n");
}

void min_heap_destroy(min_Heap h){
    free(h->array);
    free(h);
}

void min_bubble_down(min_Heap h, int root){
    //left child 2*(pos+1)-1, right child 2*(pos+1) 
    int left = (2*(root+1)-1);
    int right = 2*(root+1);
    int last = ((h->size)-1);
    if(left > last) // if it's a leaf node, dont bubble down any further
        return;

    int min;
    float l_weight, r_weight;
    
    l_weight = (h->array[left].weight); // we know this exists
    r_weight = INT_MAX;
    if(right < last){                       // check if right child exists
        r_weight = (h->array[right].weight);
    }

    // find the child with the minimum weight
    if(l_weight < r_weight)
        min = left;
    else 
        min = right;
    
    int temp;
    float temp_weight;
    int temp_index;
    if ((h->array[root].weight) > (h->array[min].weight)){
        temp = h->array[root].index;
        temp_weight = h->array[root].weight;
        h->array[root].index = h->array[min].index;
        h->array[root].weight = h->array[min].weight;
        h->array[min].index = temp;
        h->array[min].weight = temp_weight;
        min_bubble_down(h,min);
    }
}

void min_bubble_up(min_Heap h, int child){
    if (child == 0 || child < 0 || (child > h->size - 1))
        return;
    int parent = (child - 1)/2; // zero-based indexing
    int temp;
    float temp_weight;
    int temp_index;

    // swap if parent node distance is greater than child node distance
    if(h->array[parent].weight > h->array[child].weight){
        temp = h->array[parent].index;
        temp_weight = h->array[parent].weight;
        h->array[parent].index = h->array[child].index;
        h->array[parent].weight = h->array[child].weight;
        h->array[child].index = temp;
        h->array[child].weight = temp_weight;
        min_bubble_up(h,parent);
    }
    return;
}

void min_heapify(min_Heap h){
    int root = (((h->size)/2) - 1);
    while(root >= 0){
        min_bubble_down(h, root);
        root--;
    }
}
