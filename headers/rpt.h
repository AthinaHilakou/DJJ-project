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




float *find_mid_vertical_plane(void *points, int num_points, int flag);

float dot_product(float *vector1, float *vector2, int num_dimensions);

void find_indices(float *projections, float constant, int num_points,int *indices ,int *left_indices, int *right_indices, int *left_count, int *right_count);

void build_tree_parallel(rpt_Node *node, void *points, int *indices, int num_points, int flag, int num_point_limit, int thread_num);

rpt_Node* create_node(float *data, int *indices);





int **rpt_createAdjMatrix(void *points, int num_points, int flag, int num_point_limit, int thread_num);

RandomProjectionTree *rpt_tree_create(void *points, int num_points, int flag, int num_point_limit, int thread_num);

void rpt_tree_destroy(RandomProjectionTree *tree);



