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
#define DATA_LENTH_TRI 3

//Data float[100]
typedef struct mydata{
    float data_array[DATA_LENTH];
} data, *Data;



//Data int[3]
typedef struct myData_tri{
    int data_array[DATA_LENTH_TRI];
} data_tri, *Data_tri;



float dist_msr(void* array,int index_a, int index_b, int data_type);

float dist_msr_ab(void* a,void* b, int data_type);

float dist_manh(void* array,int index_a, int index_b, int data_type);

float dist_manh_ab(void* a,void* b, int data_type);

void get_weights(int *array, int data_of_interest,void* dataset, int array_lenth, 
float (* weight)(void *, int a, int b, int data_type), float *weights, int data_type);

void *import_data(char* filename, int *size);

//----------------------------------------------------------------
void *import_data_tri(char * filename, int* data_size); // "datasets/ascii/5k.orig.tri.ascii"