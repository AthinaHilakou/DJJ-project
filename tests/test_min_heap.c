#include "../headers/min_heap.h"
#include "../headers/acutest.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>


int compare_ints(int a, int b) {
	return abs(a - b);
}

void test_min_heap_create(void) {
    int data_array[] = {5, 3, 8, 2, 1};
    int data_of_interest = 1;
    float weights[5];
    for(int i = 0; i < 5; i++){
        weights[i] = (float)compare_ints(data_array[i], data_of_interest);
    }
    int array_size = 5;
    min_Heap h = min_heap_create(data_array, array_size, weights);
    TEST_CHECK(h != NULL);
    TEST_CHECK(h->size == 5);
    TEST_CHECK(h->capacity >= 5);
    TEST_CHECK(h->array[0].index == 1);
    TEST_CHECK(h->array[1].index == 2);
    TEST_CHECK(h->array[2].index == 5);
    TEST_CHECK(h->array[3].index == 3);
    TEST_CHECK(h->array[4].index == 8);

    min_heap_destroy(h);
}

void test_min_heap_pop(void) {
    int data_array[] = {5, 3, 8, 2, 1};
    int data_of_interest = 1;
    int array_size = 5;
    float weights[5];
    for(int i = 0; i < 5; i++){
        weights[i] = (float)compare_ints(data_array[i], data_of_interest);
    }
    min_Heap h = min_heap_create(data_array, array_size, weights);
    int min = min_heap_pop(h);
    TEST_CHECK(min == 1);
    TEST_CHECK(h->size == 4);
    TEST_CHECK(h->array[0].index == 2);
    TEST_CHECK(h->array[1].index == 3);
    TEST_CHECK(h->array[2].index == 5);
    TEST_CHECK(h->array[3].index == 8);
    min_heap_destroy(h);
}

void test_min_heap_insert(void) {
    int data_array[] = {5, 3, 8, 2, 1};
    int array_size = 5;
    int data_of_interest = 1;
    float weights[5];
    for(int i = 0; i < 5; i++){
        weights[i] = (float)compare_ints(data_array[i], data_of_interest);
    }
    min_Heap h = min_heap_create(data_array, array_size, weights);
    min_heap_insert(h, 6, 4);
    TEST_CHECK(h->size == 6);
    TEST_CHECK(min_heap_pop(h) == 1);
    TEST_CHECK(min_heap_pop(h) == 2);
    TEST_CHECK(min_heap_pop(h) == 3);
    TEST_CHECK(min_heap_pop(h) == 5);
    TEST_CHECK(min_heap_pop(h) == 6);
    TEST_CHECK(min_heap_pop(h) == 8);

    min_heap_destroy(h);
}

void test_min_heap_update(void) {
    int data_array[] = {5, 3, 8, 2, 1};
    int data_of_interest = 1;
    int array_size = 5;
    float weights[5];
    for(int i = 0; i < 5; i++){
        weights[i] = (float)compare_ints(data_array[i], data_of_interest);
    }
    min_Heap h = min_heap_create(data_array, array_size, weights);
    bool ret = min_heap_update(h, 4, 2);
    TEST_CHECK(ret == true);
    TEST_CHECK(h->size == 5);
    TEST_CHECK(min_heap_pop(h) == 1);
    TEST_CHECK(min_heap_pop(h) == 2);
    TEST_CHECK(min_heap_pop(h) == 3);
    TEST_CHECK(min_heap_pop(h) == 4);
    TEST_CHECK(min_heap_pop(h) == 5);
    min_heap_destroy(h);
}

TEST_LIST = {
    { "test_min_heap_create", test_min_heap_create },
    { "test_min_heap_insert", test_min_heap_insert },
    { "test_min_heap_pop", test_min_heap_pop },
    { "test_min_heap_update", test_min_heap_update },
    { NULL, NULL }
};
