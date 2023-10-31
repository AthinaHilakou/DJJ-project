#include "../headers/data.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

// reminder
// #define DATA_LENTH 100
// typedef struct mydata{
//     float data_array[DATA_LENTH];
// } data, *Data;


float readfloat(FILE *f) {
  float v;
  fread((void*)(&v), sizeof(v), 1, f);
  return v;
}



void *import_data(char* filename, int *data_size){

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }
    int fd = fileno(fp);

    uint lines;       

    fread(&lines, sizeof(__uint32_t), 1, fp);
    *data_size = lines;

    Data data_ptr = malloc(lines * sizeof(struct mydata));
    if(data_ptr == NULL){
        perror("malloc");
        exit(1);
    }

    float temp;
    
    for(int i  = 0; i < lines; i++){
        for(int j = 0; j < 100; j++){
            // Read the float values
            data_ptr[i].data_array[j] = readfloat(fp);
        }

    }

    fclose(fp);
    return data_ptr;
}

void free_data(Data *data_ptr){
    free(data_ptr);
}

float dist_msr(Data array, int index_a, int index_b){
    float sum = 0;
    for(int i = 0; i < DATA_LENTH; i++){
        sum += (array[index_a].data_array[i] - array[index_b].data_array[i])*(array[index_a].data_array[i] - array[index_b].data_array[i]);
    }
    return sqrt(sum);
}

float dist_manh(Data array,int index_a, int index_b){
    float sum = 0;
    for(int i = 0; i < DATA_LENTH; i++){
        sum += abs(array[index_a].data_array[i] - array[index_b].data_array[i]);
    }
    return sum;
}

void get_weights(int *array, int data_of_interest, Data dataset, int array_lenth, 
float (* weight)(Data, int a, int b), float *weights){
    for(int i = 0; i < array_lenth; i++){
        weights[i] = weight(dataset, data_of_interest, array[i]);
    }
}
