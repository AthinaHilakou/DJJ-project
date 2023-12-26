#include "../headers/data.h"

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


void *import_data_tri(char* filename, int* data_size){

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
    *data_size = lines;

    Data_tri data_ptr = malloc(lines * sizeof(data_tri));
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
        data_ptr[i].data_array[0] = b;
        data_ptr[i].data_array[1] = c;
        data_ptr[i].data_array[2] = d;
    }

    return data_ptr;
}

//Eucledian distance, optimisation--------------------------------------------------------------------------------
//calculate the squared norms of all the data points, store them in an array
void norms_sqred(void* array, int data_size, int data_type, float *norms){
    if(data_type == 0){
        Data d_array = (Data)array;
        for(int i = 0; i < data_size; i++){
            norms[i] = 0;
            for(int j = 0; j < DATA_LENTH; j++){
                norms[i] += d_array[i].data_array[j]*d_array[i].data_array[j];
            }
        }
    } else {
        Data_tri d_array = (Data_tri)array;
        for(int i = 0; i < data_size; i++){
            norms[i] = 0;
            for(int j = 0; j < DATA_LENTH_TRI; j++){
                norms[i] += d_array[i].data_array[j]*d_array[i].data_array[j];
            }
        }
    }
}


float dist_msr_opt(void* array, int index_a, int index_b, int data_type, float *norms){
    float sum = 0;
    if(data_type == 0){
        Data d_array = (Data)array;
        for(int i = 0; i < DATA_LENTH; i++){
            sum += (d_array[index_b].data_array[i])*(d_array[index_a].data_array[i]);
        }           
        
    } else{
        Data_tri d_array = (Data_tri)array;
        for(int i = 0; i < DATA_LENTH_TRI; i++){
            sum += (d_array[index_b].data_array[i])*(d_array[index_a].data_array[i]);
        }

    }
    float dist = norms[index_a] + norms[index_b] - 2*sum;
    return dist;
}

//------------------------------------------------------------------------------------------------------------------------



float dist_msr(void* array, int index_a, int index_b, int data_type){
    float sum = 0;
    if(data_type == 0){
        Data d_array = (Data)array;
        for(int i = 0; i < DATA_LENTH; i++){
            sum += (d_array[index_a].data_array[i] - d_array[index_b].data_array[i])*(d_array[index_a].data_array[i] - d_array[index_b].data_array[i]);
        }
    } else{
        Data_tri d_array = (Data_tri)array;
        for(int i = 0; i < DATA_LENTH_TRI; i++){
            sum += (d_array[index_a].data_array[i] - d_array[index_b].data_array[i])*(d_array[index_a].data_array[i] - d_array[index_b].data_array[i]);
        }
    }
    return sqrt(sum);
}

float dist_msr_ab(void* a, void* b, int data_type){
    float sum = 0;
    if(data_type == 0){
        Data d_a = (Data)a;
        Data d_b = (Data)b;
        for(int i = 0; i < DATA_LENTH; i++){
            sum += (d_a->data_array[i] - d_b->data_array[i])*(d_a->data_array[i] - d_b->data_array[i]);
        }
    } else {
        Data_tri d_a = (Data_tri)a;
        Data_tri d_b = (Data_tri)b;
        for(int i = 0; i < DATA_LENTH_TRI; i++){
            sum += (d_a->data_array[i] - d_b->data_array[i])*(d_a->data_array[i] - d_b->data_array[i]);
        }    
    }

    return sqrt(sum);
}

float dist_manh(void* array,int index_a, int index_b, int data_type){
    float sum = 0;
    if(data_type == 0){
        Data d_array = (Data)array;
        for(int i = 0; i < DATA_LENTH; i++){
            sum += fabs(d_array[index_a].data_array[i] - d_array[index_b].data_array[i]);
        }    
    } else {
        Data_tri d_array = (Data_tri)array;
        for(int i = 0; i < DATA_LENTH_TRI; i++){
            sum += fabs(d_array[index_a].data_array[i] - d_array[index_b].data_array[i]);
        }
    }
    return sum;
}

float dist_manh_ab(void* a, void* b, int data_type){
    float sum = 0;
    if(data_type == 0){
        Data d_a = (Data)a;
        Data d_b = (Data)b;
        for(int i = 0; i < DATA_LENTH; i++){
            sum += fabs(d_a->data_array[i] - d_b->data_array[i]);
        }
    } else {
        Data_tri d_a = (Data_tri)a;
        Data_tri d_b = (Data_tri)b;
        for(int i = 0; i < DATA_LENTH_TRI; i++){
            sum += fabs(d_a->data_array[i] - d_b->data_array[i]);
        }
    }
    return sum;
}

void get_weights(int *array, int data_of_interest, void* dataset, int array_lenth, 
float (* weight)(void* data, int a, int b, int data_type), float *weights, int data_type){
    for(int i = 0; i < array_lenth; i++){
        weights[i] = weight(dataset, data_of_interest, array[i], data_type);
    }
}
