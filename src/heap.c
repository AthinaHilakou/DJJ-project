#include "../headers/heap.h"
#include <stdio.h>
#include <stdlib.h>

Heap heap_create(void *data, void *data_of_interest,
 int (*weight_fun)(void* a, void *b), int data_size, int capacity){
    Heap h = (Heap)malloc(sizeof(heap));
    // Checking if memory is allocated
    if (h == NULL) {
        printf("Memory error");
        return NULL;
    }
    //set the value of size
    h->size = data_size;
    h->capacity = capacity;
    h->weight_fun = weight_fun;
    h->data_of_interest = data_of_interest;
    //Allocating memory for items
    h->array = (Node)malloc(capacity*sizeof(node));
 
    //Checking if memory is allocated
    if (h->array == NULL){
        printf("Memory error");
        return NULL;
    }

    //Put items in heap
    for(int i = 0; i < data_size; i++){
        h->array[i].item = data[i];
        h->array[i].weight = (*weight_fun)(data[i], data_of_interest);
    }


    //Heapify
    heapify(h);
    return h;
}

void heap_insert(Heap h, void *item){
   
   if(h->size < h->capacity){
        h->array[h->size].item = item;
        h->array[h->size].weight = h->weight_fun(h->data_of_interest, item);
        h->size++;
   }
   else if(h->size == h->capacity){
        int new_capacity = (h->capacity)*2;
        h->array = (Node)realloc(h->array, new_capacity*sizeof(node));
        h->capacity = new_capacity;
        h->array[h->size] = item;
        h->size++;
        bubble_up(h,(h->size-1));
   }

}

void *heap_pop(Heap h){
    if(h->size == 0)
        return;
    else if(h->size*4 == h->capacity){
        int new_capacity = h->capacity/2;
        h->array = (Node)realloc(h->array, new_capacity*sizeof(node));
        h->capacity = new_capacity;
    }
    void *ret;
    ret = h->array[0].item;
    h->array[0] = h->array[(h->size-1)];
    h->array[(h->size-1)] = NULL;
    h->size--;
    bubble_down(h,0);
    return ret;
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
    int left = (2*(root+1)-1);
    int right = 2*(root+1);
    int last = ((h->size)-1);
    //left child 2*(pos+1)-1, right child 2*(pos+1) 
    if (root >= last)
        return;
    if((right == last) || (left == last))
        return;

    int max;
    int l_weight = (h->array[left].weight);
    int r_weight = (h->array[right].weight);
    if(l_weight > r_weight)
        max = left;
    else 
        max = right;
    
    void *temp;
    int temp_weight;
    if ((h->array[root].weight) < (h->array[max].weight)){
        temp = h->array[root].item;
        temp_weight = h->array[root].weight;
        h->array[root].item = h->array[max].item;
        h->array[root].weight = h->array[max].weight;
        h->array[max].item = temp;
        h->array[max].weight = temp_weight;
        bubble_down(h,max);

    }
    else 
        return;
}


void bubble_up(Heap h, int child){
    if (child == 0 || child < 0 || (child > h->size - 1))
        return;
    int parent = (child - 1)/2;
    void *temp;
    int temp_weight;
    if(h->array[parent].weight < h->array[child].weight){
        temp = h->array[parent].item;
        temp_weight = h->array[parent].weight;
        h->array[parent].item = h->array[child].item;
        h->array[parent].weight = h->array[child].weight;
        h->array[child].item = temp;
        h->array[child].weight = temp_weight;
        bubble_up(h,parent);
    }
}

void heapify(Heap h){
    int root = (((h->size)/2) - 1);
    while(root >= 0){
        bubble_down(h, root);
        root--;
    }
}
