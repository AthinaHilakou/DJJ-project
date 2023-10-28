#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "graph.h"
#include "data.h"

//calculate all real neibghbors of each node and return them in a matrix
int **brute_force(int k, int (*weight)(Data *, int, int), Data *data, int data_size);

//calculate the recall value of a data point specified by index
int recall(int *aprox_KNN, int index, int k, int (*weight)(Data *, int, int), Data *data, int data_size);
