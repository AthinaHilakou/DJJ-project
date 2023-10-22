typedef struct myData_tri{
    int index;
    int x;
    int y;
    int z;
} Data_tri;

void *import_data_tri(char * filename); // "datasets/ascii/5k.orig.tri.ascii"

// euclidean distance
int dist_euc(Data_tri *a, Data_tri *b);

// mean square root
int dist_msr(Data_tri *a, Data_tri *b);

// manhattan distance
int dist_manh(Data_tri *a, Data_tri *b);