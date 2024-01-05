#include "../headers/heap.h"
#include "../headers/acutest.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>

extern float* norms;

int compare_ints(int a, int b) {
	return abs(a - b);
}

void test_heap_create(void) {
    int data_array[] = {5, 3, 8, 2, 1};
    int data_of_interest = 1;
    float weights[5];
    for(int i = 0; i < 5; i++){
        weights[i] = (float)compare_ints(data_array[i], data_of_interest);
    }
    int array_size = 5;
    Heap h = heap_create(data_array, array_size, weights);
    TEST_CHECK(h != NULL);
    TEST_CHECK(h->size == 5);
    TEST_CHECK(h->capacity >= 5);
    TEST_CHECK(h->array[0].index == 8);
    TEST_CHECK(h->array[1].index == 3);
    TEST_CHECK(h->array[2].index == 5);
    TEST_CHECK(h->array[3].index == 2);
    TEST_CHECK(h->array[4].index == 1);

    heap_destroy(h);
}

void test_heap_pop(void) {
    int data_array[] = {5, 3, 8, 2, 1};
    int data_of_interest = 1;
    int array_size = 5;
    float weights[5];
    for(int i = 0; i < 5; i++){
        weights[i] = (float)compare_ints(data_array[i], data_of_interest);
    }
    Heap h = heap_create(data_array, array_size, weights);
    int max = heap_pop(h);
    TEST_CHECK(max == 8);
    TEST_CHECK(h->size == 4);
    TEST_CHECK(h->array[0].index == 5);
    TEST_CHECK(h->array[1].index == 3);
    TEST_CHECK(h->array[2].index == 1);
    TEST_CHECK(h->array[3].index == 2);
    heap_destroy(h);
}

void test_heap_insert(void) {
    int data_array[] = {5, 3, 8, 2, 1};
    int array_size = 5;
    int data_of_interest = 1;
    float weights[5];
    for(int i = 0; i < 5; i++){
        weights[i] = (float)compare_ints(data_array[i], data_of_interest);
    }
    Heap h = heap_create(data_array, array_size, weights);
    heap_insert(h, 6, 4);
    TEST_CHECK(h->size == 6);
    TEST_CHECK(heap_pop(h) == 8);
    TEST_CHECK(heap_pop(h) == 6);
    TEST_CHECK(heap_pop(h) == 5);
    TEST_CHECK(heap_pop(h) == 3);
    TEST_CHECK(heap_pop(h) == 2);
    TEST_CHECK(heap_pop(h) == 1);

    heap_destroy(h);
}

void test_heap_update(void) {
    int data_array[] = {5, 3, 8, 2, 1};
    int data_of_interest = 1;
    int array_size = 5;
    float weights[5];
    for(int i = 0; i < 5; i++){
        weights[i] = (float)compare_ints(data_array[i], data_of_interest);
    }
    Heap h = heap_create(data_array, array_size, weights);
    bool ret = heap_update(h, 4, 2);
    TEST_CHECK(ret == true);
    TEST_CHECK(h->size == 5);
    TEST_CHECK(heap_pop(h) == 5);
    TEST_CHECK(heap_pop(h) == 4);
    TEST_CHECK(heap_pop(h) == 3);
    TEST_CHECK(heap_pop(h) == 2);
    TEST_CHECK(heap_pop(h) == 1);
    heap_destroy(h);
}

TEST_LIST = {
    { "test_heap_create", test_heap_create },
    { "test_heap_insert", test_heap_insert },
    { "test_heap_pop", test_heap_pop },
    { "test_heap_update", test_heap_update },
    { NULL, NULL }
};
