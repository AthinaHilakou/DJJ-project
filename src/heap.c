#include "../headers/heap.h"
#include <stdio.h>


Heap heap_create(int K, Node *items){
    Heap h = (Heap)malloc(sizeof(heap));
    // Checking if memory is allocated
    if (h == NULL) {
        printf("Memory error");
        return NULL;
    }
    //set the value of size
    h->size = K;
 
    //Allocating memory for items
    h->items = (Node*)malloc(K*sizeof(Node));
 
    //Checking if memory is allocated
    if (h->items == NULL) {
        printf("Memory error");
        return NULL;
    }

    //Put items in heap
    for(int i = 0; i < K; i++){
        h->items[i] = items[i];
    }


    //Heapify
    heapify(h);
    return h;
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

void heapify(Heap h){
    int root = (((h->size)/2) - 1);
    while(root >= 0){
        bubble_down(h, root);
        root--;
    }
}
