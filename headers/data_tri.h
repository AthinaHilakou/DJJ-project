#include <math.h>

typedef struct myData_tri{
    int x;
    int y;
    int z;
} Data_tri;


// euclidean distance
int dist_euc(Data_tri *a, Data_tri *b){
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

// mean square root
int dist_msr(Data_tri *a, Data_tri *b){
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2));
}

// manhattan distance
int dist_manh(Data_tri *a, Data_tri *b){
    return abs(a->x - b->x) + abs(a->y - b->y) + abs(a->z - b->z);
}
