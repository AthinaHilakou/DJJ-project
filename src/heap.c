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
//    if(weight < h->array[0].weight){
//         h->array[0].index = index;
//         h->array[0].weight = weight;
//         ret_value = true;
//         bubble_down(h, 0); //bubble down from root to maintain heap property
//    }
    if(weight < h->array[0].weight){
        int old_top = heap_pop(h);
        heap_insert(h, index, weight);
        ret_value = true;
    }
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

void heap_to_array(Heap h, int *ret_array,int *size,int flag, int* insert_flags, double sampling_rate){
    *size = 0;
    int samples = (int) h->size*sampling_rate;
    int sampled = 0;
    for(int i = 0; i < (int) h->size; i++){
        if(insert_flags[h->array[i].index] == flag){
            if(sampled < samples){
                ret_array[*size] = h->array[i].index;
                (*size)++;
                sampled++;
            }
            else{
                break;
            }
        }
    }
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
//todo no duplicates
// joins array1, and array2, returns joined array

// Merge two sorted arrays and remove duplicates
int *merge_arrays(int *array1, int size1, int *array2, int size2, int *joined_size) {
    int *joined_array = malloc((size1 + size2) * sizeof(int));
    int i = 0, j = 0, k = 0;

    // Merge the two arrays while removing duplicates
    while (i < size1 && j < size2) {
        if (array1[i] < array2[j]) {
            if (k == 0 || joined_array[k - 1] != array1[i]) {
                joined_array[k] = array1[i];
                k++;
            }
            i++;
        } else if (array2[j] < array1[i]) {
            if (k == 0 || joined_array[k - 1] != array2[j]) {
                joined_array[k] = array2[j];
                k++;
            }
            j++;
        } else {
            if (k == 0 || joined_array[k - 1] != array1[i]) {
                joined_array[k] = array1[i];
                k++;
            }
            i++;
            j++;
        }
    }

    // Copy remaining elements from array1
    while (i < size1) {
        if (k == 0 || joined_array[k - 1] != array1[i]) {
            joined_array[k] = array1[i];
            k++;
        }
        i++;
    }

    // Copy remaining elements from array2
    while (j < size2) {
        if (k == 0 || joined_array[k - 1] != array2[j]) {
            joined_array[k] = array2[j];
            k++;
        }
        j++;
    }

    *joined_size = k;
    return joined_array;
}

// Join two arrays using mergesort and remove duplicates
int *join_arrays(int *array1, int size1, int *array2, int size2, int *joined_size) {
    mergeSort(array1, 0, size1 - 1);
    mergeSort(array2, 0, size2 - 1);

    // Merge the sorted arrays and remove duplicates
    int temp = 0;
    int *joined_array = merge_arrays(array1, size1, array2, size2, &temp);
    *joined_size = temp;
    return joined_array;
}

// Merge two subarrays of arr[]
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temporary arrays
    int L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temporary arrays back into arr[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// l is for left index and r is right index of the sub-array of arr to be sorted
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}
