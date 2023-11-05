#pragma once

typedef struct myData_tri{
    int x;
    int y;
    int z;
} Data_tri;

void *import_data_tri(char * filename); // "datasets/ascii/5k.orig.tri.ascii"

void free_data_tri(Data_tri *data_ptr); // free data_ptr

// euclidean distance
int dist_euc_tri(Data_tri *a, Data_tri *b);

// mean square root
int dist_msr_tri(Data_tri *a, Data_tri *b);

// manhattan distance
int dist_manh_tri(Data_tri *a, Data_tri *b);