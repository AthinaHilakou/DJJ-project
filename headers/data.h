#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <math.h>

#pragma once
#define DATA_LENTH 100
typedef struct mydata{
    float data_array[DATA_LENTH];
} data, *Data;



int dist_msr(Data array,int index_a, int index_b);

int dist_manh(Data array,int index_a, int index_b);

int *get_weights(int *array, int data_of_interest, Data dataset, int array_lenth, int (* weight)(Data , int a, int b));

void *import_data(char* filename, int *size);
