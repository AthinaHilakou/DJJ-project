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

void test_map_add(void) {
    Map m = map_init(3);
    // add first element
    map_add(m, 1, 2);
    TEST_CHECK(m->size == 1);
    TEST_CHECK(m->capacity == 3);
    // add second element, forcing reshashing
    map_add(m, 2, 3);
    TEST_CHECK(m->size == 2);
    TEST_CHECK(m->capacity == 6);
    // add third element
    map_add(m, 3, 4);
    TEST_CHECK(m->size == 3);
    TEST_CHECK(m->capacity == 6);

    TEST_CHECK(map_get(m, 1) == 2);
    TEST_CHECK(map_get(m, 2) == 3);
    TEST_CHECK(map_get(m, 3) == 4);
    TEST_CHECK(map_get(m, 4) == -1);

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
    mapify(m, array, weights, 3);
    TEST_CHECK(m->size == 3);
    TEST_CHECK(m->capacity == 10);
    TEST_CHECK(map_get(m, 1) == 1);
    TEST_CHECK(map_get(m, 2) == 2);
    TEST_CHECK(map_get(m, 3) == 3);
    map_destroy(m);
}


void test_map_remove(void){
    Map m = map_init(10);
    int array[] = {1, 2, 3};
    float weights[] = {1, 2, 3};
    mapify(m, array, weights, 3);
    TEST_CHECK(m->size == 3);
    TEST_CHECK(m->capacity == 10);
    TEST_CHECK(map_remove(m, 1) == true);
    TEST_CHECK(m->size == 2);
    TEST_CHECK(map_remove(m, 2) == true);
    TEST_CHECK(m->size == 1);
    TEST_CHECK(map_remove(m, 3) == true);
    TEST_CHECK(m->size == 0);
    TEST_CHECK(map_remove(m, 4) == false);
    TEST_CHECK(m->size == 0);
    TEST_CHECK(map_remove(m, 1) == false);
    TEST_CHECK(m->size == 0);
    map_destroy(m);
}

void test_map_update(void){
    Map m = map_init(10);
    int array[] = {1, 2, 3};
    float weights[] = {1, 2, 3};
    mapify(m, array, weights, 3);
    TEST_CHECK(m->size == 3);
    TEST_CHECK(m->capacity == 10);
    TEST_CHECK(map_update(m, 4, 4, 1) == true);
    TEST_CHECK(m->size == 3);
    TEST_CHECK(map_get(m, 1) == -1);
    TEST_CHECK(map_get(m, 4) == 4);
    TEST_CHECK(map_update(m, 4, 4, 2) == false);
    TEST_CHECK(map_get(m, 2) == 2);
    TEST_CHECK(map_get(m, 4) == 4);
    TEST_CHECK(m->size == 3);
    TEST_CHECK(map_update(m, 4, 4, 3) == false);
    TEST_CHECK(m->size == 3);
    TEST_CHECK(map_update(m, 4, 4, 4) == false);
    TEST_CHECK(m->size == 3);
    map_destroy(m);
}


TEST_LIST = {
    {"hash", test_hash},
    {"map_init", test_map_init},
    {"map_add", test_map_add},
    {"map_get", test_map_get},
    {"mapify", test_mapify},
    {"map_remove", test_map_remove},
    {"map_update", test_map_update},
    {NULL, NULL}
};
