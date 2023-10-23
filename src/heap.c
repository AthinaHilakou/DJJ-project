#include "../headers/heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Heap heap_create(void *data_array, void *data_of_interest,
 int (*weight_fun)(void* a, void *b), int array_size, int capacity, int data_size){
    Heap h = (Heap)malloc(sizeof(heap));
    // Checking if memory is allocated
    if (h == NULL) {
        printf("Memory error");
        return NULL;
    }
    //set the value of size
    h->size = array_size;   // size of input array
    h->capacity = capacity; // current max size of heap
    h->weight_fun = weight_fun; // weight function specific to the data type
    h->data_of_interest = data_of_interest; // the "owner" of the heap
    //Allocating memory for items
    h->array = (Node)malloc(capacity*sizeof(node));

    //Checking if memory is allocated
    if (h->array == NULL){
        printf("Memory error");
        return NULL;
    }

    //Put items in heap
    for(int i = 0; i < array_size; i++){
        h->array[i].item = data_array + i*data_size;
        h->array[i].weight = (*weight_fun)(data_array + i*data_size, data_of_interest);
    }


    //Heapify
    heapify(h);
    return h;
}

void heap_insert(Heap h, void *item){
   
   if(h->size < h->capacity){
        h->array[h->size].item = item;  // insert item
        h->array[h->size].weight = h->weight_fun(h->data_of_interest, item);    // calculate weight
        h->size++;                  // update size of heap
        bubble_up(h,(h->size-1));   // restore heap property
   }
   else if(h->size == h->capacity){
        int new_capacity = (h->capacity)*2; // double capacity
        h->array = (Node)realloc(h->array, new_capacity*sizeof(node));  // reallocate memory
        h->capacity = new_capacity; // update capacity
        Node temp = NULL;
        h->array[h->size] = *temp; // insert item
        h->size++;  // update size of heap
        bubble_up(h,(h->size-1)); // restore heap property
   }

}

void *heap_pop(Heap h){
    if(h->size == 0)
        return NULL;
    else if(h->size*4 == h->capacity){
        int new_capacity = h->capacity/2;
        h->array = (Node)realloc(h->array, new_capacity*sizeof(node));
        h->capacity = new_capacity;
    }
    void *ret;
    ret = h->array[0].item;
    h->array[0] = h->array[(h->size-1)];
    Node temp = NULL;
    h->array[(h->size-1)] = *temp;
    h->size--;
    bubble_down(h,0);
    return ret;
}

int get_heap_size(Heap h){
    return h->size;
}

void get_heap_item_from_index(Heap h, int index){
    return h->array[index].item;
}

bool heap_update(Heap h, void *item, void *old_root){
   bool ret_value = false;
   old_root = NULL;  
   int item_weight = h->weight_fun(h->data_of_interest, item);
   if(item_weight < h->array[0].weight){
        old_root = h->array[0].item;
        h->array[0].item = item;
        h->array[0].weight = item_weight;
        ret_value = true;
        bubble_down(h, 0); //bubble down from root to maintain heap property
   }
   return ret_value;
}

void *heap_find_max(Heap h){
    return h->array[0].item;
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
    if(left > last) // leaf node, no children
        return;

    int min;
    int l_weight = INT_MAX;
    if(left < last){                        // if left child exists
        l_weight = (h->array[left].weight);
    }
    int r_weight = INT_MAX;
    if(right < last){                       // if right child exists
        r_weight = (h->array[right].weight);
    }

    // find the child with the minimum weight
    if(l_weight > r_weight)
        min = left;
    else 
        min = right;
    
    void *temp;
    int temp_weight;
    int temp_index;
    if ((h->array[root].weight) > (h->array[min].weight)){
        temp = h->array[root].item;
        temp_weight = h->array[root].weight;
        h->array[root].item = h->array[min].item;
        h->array[root].weight = h->array[min].weight;
        h->array[min].item = temp;
        h->array[min].weight = temp_weight;
        bubble_down(h,min);
    }
}

void bubble_up(Heap h, int child){
    if (child == 0 || child < 0 || (child > h->size - 1))
        return;
    int parent = (child - 1)/2; // zero-based indexing
    void *temp;
    int temp_weight;
    int temp_index;

    // swap if parent node distance is greater than child node distance
    if(h->array[parent].weight > h->array[child].weight){
        temp = h->array[parent].item;
        temp_weight = h->array[parent].weight;
        h->array[parent].item = h->array[child].item;
        h->array[parent].weight = h->array[child].weight;
        h->array[child].item = temp;
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
