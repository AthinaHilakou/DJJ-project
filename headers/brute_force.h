#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "graph.h"
#include "data.h"

//calculate all real neibghbors of each node and return them in a matrix
// int **brute_force(int k, float (*weight)(Data *, int, int), Data *data, int data_size);

//calculate the recall value of a data point specified by index
int **brute_force(int k, float (*weight)(void* , int, int, int), void* data, int data_size, int flag);
float recall(int **aprox_KNN, int k, float (*weight)(void* , int, int, int), void* data, int data_size, int flag);
void search(int **graph, float weight_fun(void* a, void* b, int), void* my_data, int data_size, int k, int**all_neighbors, int *sizes, int flag);
