#include "../headers/map.h"
#include "../headers/acutest.h"
#include <stdlib.h>
#include <stdio.h>

void test_hash(){
    TEST_CHECK(hash(1, 10) == 1);
    TEST_CHECK(hash(2, 10) == 2);
    TEST_CHECK(hash(14, 10) == 4);
    TEST_CHECK(hash(15, 10) == 5);
    TEST_CHECK(hash(26, 10) == 6);
    TEST_CHECK(hash(127, 100) == 27);
}

void test_map_init(void) {
    Map m = map_init(10);
    TEST_CHECK(m != NULL);
    TEST_CHECK(m->size == 0);
    TEST_CHECK(m->capacity == 10);
    map_destroy(m);
}

void map_add(void) {
    Map m = map_init(2);
    // add first element
    map_add(m, 1, 2);
    TEST_CHECK(m->size == 1);
    TEST_CHECK(m->capacity == 2);
    // add second element
    map_add(m, 2, 3);
    TEST_CHECK(m->size == 2);
    TEST_CHECK(m->capacity == 2);
    // add third element, forcing reshashing
    map_add(m, 3, 4);
    TEST_CHECK(m->size == 3);
    TEST_CHECK(m->capacity == 4);
    map_destroy(m);
}

void test_map_get(void) {
    Map m = map_init(2);
    map_add(m, 1, 2);
    map_add(m, 2, 3);
    map_add(m, 3, 4);
    TEST_CHECK(map_get(m, 1) == 2);
    TEST_CHECK(map_get(m, 2) == 3);
    TEST_CHECK(map_get(m, 3) == 4);
    TEST_CHECK(map_get(m, 4) == -1);
    map_destroy(m);
}

void test_mapify(void) {
    Map m = map_init(10);
    int array[] = {1, 2, 3};
    float weights[] = {1, 2, 3};
    mapify(m, array, 3);
    TEST_CHECK(m->size == 3);
    TEST_CHECK(m->capacity == 10);
    TEST_CHECK(map_get(m, 1) == 1);
    TEST_CHECK(map_get(m, 2) == 2);
    TEST_CHECK(map_get(m, 3) == 3);
    map_destroy(m);
}


TEST_LIST = {
    { "test_heap_create", test_heap_create },
    { "test_heap_insert", test_heap_insert },
    { "test_heap_pop", test_heap_pop },
    { "test_heap_update", test_heap_update },
    { NULL, NULL }
};
