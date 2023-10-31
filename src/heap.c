#include "../headers/heap.h"
#include "../headers/data.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Heap heap_create(int *data_array, int array_size, float *weights){
    Heap h = (Heap)malloc(sizeof(heap));
    // Checking if memory is allocated
    if (h == NULL) {
        printf("Memory error");
        return NULL;
    }
    //set the value of size
    h->size = array_size;   // size of input array
    h->capacity = 3*array_size/2 + 10; // current max size of heap
    //Allocating memory for items
    h->array = (Node)malloc((h->capacity)*sizeof(node));

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
    heapify(h);
    return h;
}

void heap_insert(Heap h, int index, float weight){
   
   if(h->size < h->capacity){
        h->array[h->size].index = index;  // insert item
        h->array[h->size].weight = weight; // insert weight
        h->size++;                  // update size of heap
        bubble_up(h,(h->size-1));   // restore heap property
   }
   else if(h->size == h->capacity){
        int new_capacity = (h->capacity)*2; // double capacity
        h->array = (Node)realloc(h->array, new_capacity*sizeof(node));  // reallocate memory
        h->capacity = new_capacity; // update capacity
        h->array[h->size].index = index; // insert item
        h->array[h->size].weight = weight; // insert weight
        h->size++;  // update size of heap
        bubble_up(h,(h->size-1)); // restore heap property
   }
}

int heap_pop(Heap h){
    if(h->size == 0)
        return -1;
    else if(h->size*4 == h->capacity){
        int new_capacity = h->capacity/2;
        h->array = (Node)realloc(h->array, new_capacity*sizeof(node));
        h->capacity = new_capacity;
    }
    int ret;
    ret = h->array[0].index;
    h->array[0].index = h->array[(h->size-1)].index;
    h->array[0].weight = h->array[(h->size-1)].weight;
    h->size--;
    bubble_down(h,0);
    return ret;
}

int get_heap_size(Heap h){
    return h->size;
}

int index_from_heap(Heap h, int node_index){
    return h->array[node_index].index;
}

bool heap_update(Heap h, int index, float weight){
    //printf("index = %d, weight = %f\n", index, weight);
    //printf("h->array[0].index = %d, h->array[0].weight = %f\n", h->array[0].index, h->array[0].weight);
   bool ret_value = false;  
   if(weight < h->array[0].weight){
        h->array[0].index = index;
        h->array[0].weight = weight;
        ret_value = true;
        bubble_down(h, 0); //bubble down from root to maintain heap property
   }
    // if(weight < h->array[0].weight){
    //     int old_top = heap_pop(h);
    //     heap_insert(h, index, weight);
    //     ret_value = true;
    // }
   return ret_value;
}

int heap_find_max(Heap h){
    return h->array[0].index;
}


void print_heap(Heap h){
    printf("Heap start \n");
    for(int i = 0; i < h->size; i++){
        printf("index: %d, weight: %f\n", h->array[i].index, h->array[i].weight);
    }
    printf("Heap end \n");
}

void heap_destroy(Heap h){
    free(h->array);
    free(h);
}

void bubble_down(Heap h, int root){
    //left child 2*(pos+1)-1, right child 2*(pos+1) 
    int left = (2*(root+1)-1);
    int right = 2*(root+1);
    int last = ((h->size)-1);
    if(left > last) // if it's a leaf node, skip this
        return;

    int max;
    float l_weight, r_weight;
    
    l_weight = (h->array[left].weight); // we know this exists
    r_weight = INT_MIN;
    if(right < last){                       // check if right child exists
        r_weight = (h->array[right].weight);
    }

    // find the child with the maximum weight
    if(l_weight > r_weight)
        max = left;
    else 
        max = right;
    
    int temp;
    float temp_weight;
    int temp_index;
    if ((h->array[root].weight) < (h->array[max].weight)){
        temp = h->array[root].index;
        temp_weight = h->array[root].weight;
        h->array[root].index = h->array[max].index;
        h->array[root].weight = h->array[max].weight;
        h->array[max].index = temp;
        h->array[max].weight = temp_weight;
        bubble_down(h,max);
    }
}

void bubble_up(Heap h, int child){
    if (child == 0 || child < 0 || (child > h->size - 1))
        return;
    int parent = (child - 1)/2; // zero-based indexing
    int temp;
    float temp_weight;
    int temp_index;

    // swap if parent node distance is greater than child node distance
    if(h->array[parent].weight < h->array[child].weight){
        temp = h->array[parent].index;
        temp_weight = h->array[parent].weight;
        h->array[parent].index = h->array[child].index;
        h->array[parent].weight = h->array[child].weight;
        h->array[child].index = temp;
        h->array[child].weight = temp_weight;
        bubble_up(h,parent);
    }
    return;
}

void heapify(Heap h){
    int root = (((h->size)/2) - 1);
    while(root >= 0){
        bubble_down(h, root);
        root--;
    }
}
