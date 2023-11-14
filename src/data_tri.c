#include "../headers/my_data_tri.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>


void *import_data_tri(char* filename){

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
    int lines = size / (41*sizeof(char));       // each line has 40 chars and '\n

    Data_tri *data_ptr = malloc(lines * sizeof(Data_tri));
    if(data_ptr == NULL){
        perror("malloc");
        exit(1);
    }

    int a, b, c, d;
    for(int i  = 1; i < lines; i++){

        if (fseek(fp, (i - 1) * 41 * sizeof(char), SEEK_SET) != 0) {
            perror("fseek");
            exit(1);
        }
        if (fscanf(fp, "%d %d %d %d", &a, &b, &c, &d) != 4) {
            perror("fscanf");
            exit(1);
        }
        // a is the index, we dont need it
        data_ptr[i].x = b;
        data_ptr[i].y = c;
        data_ptr[i].z = d;

    }

    return data_ptr;
}

void free_data_tri(Data_tri *data_ptr){
    free(data_ptr);
}

// mean square root
int dist_msr_tri(Data_tri *a, Data_tri *b){
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2));
}

// manhattan distance
int dist_manh_tri(Data_tri *a, Data_tri *b){
    return abs(a->x - b->x) + abs(a->y - b->y) + abs(a->z - b->z);
}
