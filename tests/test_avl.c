#include "../headers/avl_tree.h"
#include "../headers/acutest.h"
#include <stdlib.h>
#include <stdio.h>

void test_max(){
    TEST_CHECK(max(127, 100) == 127);
    TEST_CHECK(max(100, 127) == 127);
    TEST_CHECK(max(100, 100) == 100);
    TEST_CHECK(max(0, 50) == 50);
    TEST_CHECK(max(50, 0) == 50);
    TEST_CHECK(max(0, 0) == 0);
    TEST_CHECK(max(-1, 0) == 0);
}

void test_avl_create(void){
    Avl_tree a_tree = avl_create();
    TEST_CHECK(a_tree != NULL);
    TEST_CHECK(a_tree->size == 0);
    avl_destroy(a_tree);
}

void test_avl_insert(void) {
    Avl_tree a_tree = avl_create();
    avl_insert(a_tree, 1, 2, 1);
    avl_insert(a_tree, 2, 3, 1);
    avl_insert(a_tree, 3, 4, 1);
    TEST_CHECK(a_tree->size == 3);
    TEST_CHECK(a_tree->root->key == 2);
    TEST_CHECK(a_tree->root->left->key == 1);
    TEST_CHECK(a_tree->root->right->key == 3);
    avl_destroy(a_tree);
}


void test_avl_remove(void) {
    Avl_tree a_tree = avl_create();
    avl_insert(a_tree, 1, 2, 1);
    avl_insert(a_tree, 2, 3, 1);
    avl_insert(a_tree, 3, 4, 1);
    avl_remove(a_tree, 1);
    TEST_CHECK(a_tree->size == 2);
    TEST_CHECK(a_tree->root->key == 2);
    TEST_CHECK(a_tree->root->left == NULL);
    TEST_CHECK(a_tree->root->right->key == 3);
    avl_remove(a_tree, 2);
    TEST_CHECK(a_tree->size == 1);
    TEST_CHECK(a_tree->root->key == 3);
    TEST_CHECK(a_tree->root->left == NULL);
    TEST_CHECK(a_tree->root->right == NULL);
    avl_remove(a_tree, 3);
    TEST_CHECK(a_tree->size == 0);
    TEST_CHECK(a_tree->root == NULL);
    avl_destroy(a_tree);
}

void test_avl_from_array(void) {
    int array[] = {1, 2, 3};
    float weights[] = {1, 2, 3};
    Avl_tree a_tree = avl_create();
    avl_from_array(a_tree,array, weights, 3);
    TEST_CHECK(a_tree->size == 3);
    TEST_CHECK(a_tree->root->key == 2);
    TEST_CHECK(a_tree->root->value == 2);
    TEST_CHECK(a_tree->root->flag == 1);
    TEST_CHECK(a_tree->root->left->key == 1);
    TEST_CHECK(a_tree->root->left->value == 1);
    TEST_CHECK(a_tree->root->left->flag == 1);
    TEST_CHECK(a_tree->root->right->key == 3);
    TEST_CHECK(a_tree->root->right->value == 3);
    TEST_CHECK(a_tree->root->right->flag == 1);
    avl_destroy(a_tree);
}


void test_avl_to_array(void){
    Avl_tree a_tree = avl_create();
    int size;
    avl_insert(a_tree, 1, 2, 1);
    avl_insert(a_tree, 2, 3, 1);
    avl_insert(a_tree, 3, 4, 1);
    int *array = avl_to_array(a_tree, &size, 1, 1,a_tree->size);
    TEST_CHECK(array[0] == 1);
    TEST_CHECK(array[1] == 2);
    TEST_CHECK(array[2] == 3);
    free(array);
    avl_destroy(a_tree);
}

void test_avl_get_weight(void){
    Avl_tree a_tree = avl_create();
    avl_insert(a_tree, 1, 2, 1);
    avl_insert(a_tree, 2, 3, 1);
    avl_insert(a_tree, 3, 4, 1);
    avl_insert(a_tree, 4, 5, 1);
    TEST_CHECK(avl_get_weight(a_tree, 1) == 2);
    TEST_CHECK(avl_get_weight(a_tree, 2) == 3);
    TEST_CHECK(avl_get_weight(a_tree, 3) == 4);
    TEST_CHECK(avl_get_weight(a_tree, 4) == 5);
    TEST_CHECK(avl_get_weight(a_tree, 10) == -1);
    
    avl_remove(a_tree, 1);
    TEST_CHECK(avl_get_weight(a_tree, 1) == -1);
    
    avl_destroy(a_tree);
}


TEST_LIST = {
    {"max", test_max},
    {"test_avl_create", test_avl_create},
    {"test_avl_insert", test_avl_insert},
    {"test_avl_remove", test_avl_remove},
    {"test_avl_from_array", test_avl_from_array},
    {"test_avl_to_array", test_avl_to_array},
    {"test_avl_get_weight", test_avl_get_weight},
    {NULL, NULL}
};
