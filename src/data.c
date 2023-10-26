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



void *import_data(char* filename, int *data_size){

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    struct stat sb;
    if (fstat(fileno(fp), &sb) == -1) {
        perror("fstat");
        exit(1);
    }
    size_t size = sb.st_size;
    uint lines;       // each line has 40 chars and '\n

    fread(&lines, sizeof(lines), 1, fp);
    printf("lines are %d\n", lines);
    *data_size = lines;
    return 0;

    // Data *data_ptr = malloc(lines * sizeof(Data_tri));
    // if(data_ptr == NULL){
    //     perror("malloc");
    //     exit(1);
    // }

    // int a, b, c, d;
    // for(int i  = 1; i < lines; i++){

    //     if (fseek(fp, (i - 1) * 41 * sizeof(char), SEEK_SET) != 0) {
    //         perror("fseek");
    //         exit(1);
    //     }
    //     if (fscanf(fp, "%d %d %d %d", &a, &b, &c, &d) != 4) {
    //         perror("fscanf");
    //         exit(1);
    //     }
    //     // a is the index, we dont need it
    //     data_ptr[i].x = b;
    //     data_ptr[i].y = c;
    //     data_ptr[i].z = d;

    // }

    // return data_ptr;
}

void free_data(Data *data_ptr){
    free(data_ptr);
}

int dist_msr(Data array,int index_a, int index_b){
    int sum = 0;
    for(int i = 0; i < DATA_LENTH; i++){
        sum += (array[index_a].data_array[i] - array[index_b].data_array[i])*(array[index_a].data_array[i] - array[index_b].data_array[i]);
    }
    return sqrt(sum);
}

int dist_manh(Data array,int index_a, int index_b){
    int sum = 0;
    for(int i = 0; i < DATA_LENTH; i++){
        sum += abs(array[index_a].data_array[i] - array[index_b].data_array[i]);
    }
    return sum;
}

int *get_weights(int *array, int data_of_interest, Data dataset, int array_lenth, int (* weight)(Data, int a, int b)){
    int *weights = (int *)malloc(array_lenth*sizeof(int));
    for(int i = 0; i < array_lenth; i++){
        weights[i] = weight(dataset, data_of_interest, array[i]);
    }
    return weights;
}
