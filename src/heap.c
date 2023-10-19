#include "../headers/heap.h"
#include <stdio.h>
#include <stdlib.h>

Heap heap_create(Node *items, int size, int capacity){
    Heap h = (Heap)malloc(sizeof(heap));
    // Checking if memory is allocated
    if (h == NULL) {
        printf("Memory error");
        return NULL;
    }
    //set the value of size
    h->size = size;
    h->capacity = capacity;
    //Allocating memory for items
    h->items = (Node*)malloc(capacity*sizeof(Node));
 
    //Checking if memory is allocated
    if (h->items == NULL) {
        printf("Memory error");
        return NULL;
    }

    //Put items in heap
    for(int i = 0; i < capacity; i++){
        h->items[i] = items[i];
    }


    //Heapify
    heapify(h);
    return h;
}

void heap_insert(Heap h, Node item){
   
   if(h->size < h->capacity){
        h->items[h->size] = item;
        h->size++;
   }
   else if(h->size == h->capacity){
        int new_capacity = (h->capacity)*2;
        h->items = (Node*)realloc(h->items, new_capacity*sizeof(Node));
        h->capacity = new_capacity;
        h->items[h->size] = item;
        h->size++;
        bubble_up(h,(h->size-1));
   }

}

Node heap_pop(Heap h){
    if(h->size == 0)
        return;
    else if(h->size*2 == h->capacity){
        int new_capacity = h->capacity/2;
        h->items = (Node*)realloc(h->items, new_capacity*sizeof(Node));
        h->capacity = new_capacity;
    }
    Node ret;
    ret = h->items[0];
    h->items[0] = h->items[(h->size-1)];
    h->items[(h->size-1)] = NULL;
    h->size--;
    bubble_down(h,0);
    return ret;
}



Node heap_update(Heap h, Node item){
   Node old_root = h->items[0];
   h->items[0] = item;
   bubble_down(h, 0); //bubble down from root to maintain heap property
   return old_root;
}

Node heap_find_max(Heap h){
    return h->items[0];
}


void heap_destroy(Heap h){
    free(h->items);
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
    int l_weight = (h->items[left]->weight);
    int r_weight = (h->items[right]->weight);
    if(l_weight > r_weight)
        max = left;
    else 
        max = right;
    
    Node temp;
    if ((h->items[root]->weight) < (h->items[max]->weight)){
        temp = h->items[root];
        h->items[root] = h->items[max];
        h->items[max] = temp;
        bubble_down(h,max);

    }
    else 
        return;
}


void bubble_up(Heap h, int child){
    if (child == 0 || child < 0 || (child > h->size - 1))
        return;
    int parent = (child - 1)/2;
    Node temp;
    if(h->items[parent]->weight < h->items[child]->weight){
        temp = h->items[parent];
        h->items[parent] = h->items[child];
        h->items[child] = temp;
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
