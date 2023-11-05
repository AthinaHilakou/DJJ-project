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



float dist_msr(Data array,int index_a, int index_b);

float dist_msr_ab(data a,data b);

float dist_manh(Data array,int index_a, int index_b);

float dist_manh_ab(data a,data b);

void get_weights(int *array, int data_of_interest, Data dataset, int array_lenth, 
float (* weight)(Data, int a, int b), float *weights);

void *import_data(char* filename, int *size);
