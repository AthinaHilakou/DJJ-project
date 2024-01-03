#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../headers/data.h"
#include <string.h>
#include <omp.h>

#define NUM_POINTS 1000
#define NUM_DIMENSIONS 100


typedef struct my_rpt_Node {
    void *data;     //this is the data of file
    int *indices;   // we want to split this data into two groups
    int num_points_limit; // stop when we reach this number of points
    struct my_rpt_Node *left;
    struct my_rpt_Node *right;
} rpt_Node;

typedef struct RandomProjectionTree {
    rpt_Node *root;
    int num_points_limit;
    int data_type_flag; //TODO 0 for data, 1 for data_tri
} RandomProjectionTree;

// returns midplane between two points at mid_plane
float *find_mid_vertical_plane(void *points, int num_points, int flag){

    if(num_points <= 2){
        printf("Error: no points to find midplane\n");
        exit(9);
    }
    int num_dimensions = 0;
    if(flag == 0){
        Data d_array = (Data)points;
        num_dimensions = 100;
    } else{
        Data_tri d_array = (Data_tri)points;
        num_dimensions = 3;
    }

    int index_a = rand() % num_points;
    int index_b = rand() % num_points;
    while(index_a == index_b){
        index_b = rand() % num_points;
    }

    float *point1 = d_array[index_a].data_array;
    float *point2 = d_array[index_b].data_array;

    // we will return this
    float *hyperplane = (float *)malloc((num_dimensions + 1) * sizeof(float));
    //find vector of two points
    for (int i = 0; i < num_dimensions; ++i) {
        hyperplane[i] = point2[i] - point1[i];
    }
    //find midplane
    float mid_plane[num_dimensions];
    for (int i = 0; i < num_dimensions; ++i) {
        mid_plane[i] = (point1[i] + point2[i]) / 2.0;
    }
    //find a vector whose dot product with vector is 0, and has midplane as a point    
    float constant = 0;
    for (int i = 0; i < num_dimensions; ++i) {
        constant += mid_plane[i] * hyperplane[i];
    }
    hyperplane[num_dimensions] = constant;
    
    return hyperplane;
}

float dot_product(float *vector1, float *vector2, int num_dimensions){
    float result = 0.0;
    for (int i = 0; i < num_dimensions; ++i) {
        result += vector1[i] * vector2[i];
    }

    return result;
}
//find indices of points that belong in one of the two spaces in which the hyperplane divides the whole
//any indeces left out belong to the other space
void find_indices(float *projections, float constant, int num_points, int *left_indices, int *right_indices, int *left_count, int *right_count){
    right_indices = (int *)malloc(num_points * sizeof(int));
    left_indices = (int *)malloc(num_points * sizeof(int));
    for (int i = 0; i < num_points; ++i) {
        if (projections[i] <= constant) {
            right_indices[(*right_count)++] = i;
        } else{
            left_indices[(*left_count)++] = i;
        }
    }

    if(*left_count == 0 || *right_count == 0){
        for(int i = 0; i < num_points; i++){
            if(rand() % 2 == 0){
                left_indices[(*left_count)++] = i;
            } else{
                right_indices[(*right_count)++] = i;
            }
        }
    }
    
    left_indices = (int *)realloc(left_indices, *left_count * sizeof(int));
    right_indices = (int *)realloc(right_indices, *right_count * sizeof(int));
}


//Create a
rpt_Node* create_node(float *data, int *indices) {
    rpt_Node *node = (rpt_Node *)malloc(sizeof(rpt_Node));
    node->data = data;
    node->indices = indices;
    node->left = NULL;
    node->right = NULL;
    return node;
}


void build_tree_parallel(rpt_Node *node, void *points, int *indices, int num_points, int flag, int num_point_limit) {
    //TODO add flag for data tri and handle then differently, replace num_dimensions with flag

    // this is all points, everything
    node->data = points;
    int num_dimensions;
    if(flag == 0){
        Data d_array = (Data)node->data;
        num_dimensions = 100;
    } else{
        Data_tri d_array = (Data_tri)node->data;
        num_dimensions = 3;
    }
    node->indices = indices;
    node->num_points_limit = num_point_limit;

    if (num_points < num_point_limit) {
        return;
    }

    // find perpendicular bisector hyperplane
    float *random_hyperplane = find_mid_vertical_plane(points,num_points,flag);
    
    // find projections of all points on this hyperplane(dot products)
    float *projections = (float *)malloc(num_points * sizeof(float));

    #pragma omp parallel for schedule(dynamic) // Use dynamic scheduling
    for (int i = 0; i < num_points; ++i) {
        projections[i] = dot_product(d_array[indices[i]].data_array, random_hyperplane, num_dimensions);
    }

    float constant = random_hyperplane[num_points];

    // // nth_element(projections, projections + median_index, projections + num_points);
    // median_projection = projections[median_index];

    // int *left_indices = find_indices(projections, median_projection, num_points);
    // int *right_indices = find_indices(projections, median_projection, num_points - left_count);

    int left_count = 0;
    int right_count = 0;

    int *left_indices;
    int *right_indices;
    find_indices(projections,constant, num_points, left_indices, right_indices, &left_count, &right_count);
    if(left_count == 0 || right_count == 0){
            //DO Stuff
    }
    //We stopped here--------------------------------------------------------------------------------------------
    free(projections);

    node->data = random_hyperplane;
    node->indices = left_indices;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            node->left = create_node(NULL, NULL);  // Left child
            build_tree_parallel(node->left, points, left_indices, left_count, flag, num_point_limit);
        }

        #pragma omp section
        {
            node->right = create_node(NULL, NULL);  // Right child
            build_tree_parallel(node->right, points, right_indices, right_count, flag, num_point_limit);
        }
    }
    
}

void build_parallel(RandomProjectionTree *tree, void *points, int num_points, int flag, int num_point_limit) {
    tree->root = create_node(NULL, NULL);
    int *indices = (int *)malloc(num_points * sizeof(int));
    for(int i = 0; i < num_points; i++){
        indices[i] = i;
    }
    int num_dimensions;
    if(flag == 0){
        num_dimensions = ;
    } else{
        num_dimensions = 3;
    }
    build_tree_parallel(tree->root, points, indices, num_points, num_dimensions, num_point_limit);
}

void get_arguments(int argc, char** argv, int *maxNeighbors, float (**weight_fun)(void *, int, int, int), int *data_size, void** data, int* flag, double* delta, double* sampling_rate){
    if(argc < 5){
        //              0      1              2           3                4      5       6
        printf("Usage: ./main <maxNeighbors> <file_name> <weight_function> <flag> <delta> <sampling_rate>\n");
        exit(1);
    }
    *maxNeighbors = atoi(argv[1]);
    *flag = atoi(argv[4]);
    *delta = strtod(argv[5], NULL);
    *sampling_rate = strtod(argv[6], NULL);
    
    if(*flag == 0){  // competition data case
        *data = (Data)import_data(argv[2], data_size);
        if(*data == NULL){
            printf("Error in import_data\n");
            exit(1);
        }
    } else if(*flag == 1){   // ascii data case
        *data = (Data_tri)import_data_tri(argv[2], data_size);
    } else{ // false flag case
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag> <delta>\nso that flag = 0 || 1 \n");
        exit(1);
    }
    if(strcmp(argv[3], "manh") == 0){
        *weight_fun = dist_manh;
    } else if(strcmp(argv[3], "eucl") == 0){
        *weight_fun = dist_msr;
    } else if(strcmp(argv[3], "eucl_opt") == 0){
        *weight_fun = dist_msr_opt;
    }else{
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag>\nso that weight_fun = manh || eucl || eucl_opt\n");
        exit(1);
    }

    if(*delta > 1.0 || *delta <= 0){
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag> <delta>\nso that delta is in (0,1] \n");
        exit(1);
    }
    if(*sampling_rate > 1.0 || *sampling_rate <= 0){
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag> <delta> <sampling_rate>\nso that sampling_rate is in (0,1] \n");
        exit(1);
    }

}

int main(int argc, char** argv){
    srand(348); // seed random number generator
    int maxNeighbors;
    float (*weight_fun)(void*, int, int, int);
    printf("Starting KNN aproximation\n");
    int data_size;
    void** data_p = malloc(sizeof(void *));
    int flag;
    double delta = 0.0001; //default value of delta parameter
    double sampling_rate = 0.4; //default value of sampling_rate parameter

    get_arguments(argc, argv, &maxNeighbors, &weight_fun, &data_size, data_p, &flag, &delta, &sampling_rate);
    void* data = *data_p;

    RandomProjectionTree tree;

    float start_time = omp_get_wtime();
    build_parallel(&tree, data, NUM_POINTS, NUM_DIMENSIONS);
    float end_time = omp_get_wtime();

    printf("Time taken: %f seconds\n", end_time - start_time);

    // ... (rest of the code remains the same)

    return 0;
}
