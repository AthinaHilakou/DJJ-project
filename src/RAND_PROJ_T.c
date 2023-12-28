#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../headers/data.h"
#include <omp.h>


typedef struct my_rpt_Node {
    void *data;     //this is the data of file
    int *indices;   // we want to split this data into two groups
    int num_points_limit; // stop when we reach this number of points
    struct Node *left;
    struct Node *right;
} rpt_Node;

typedef struct RandomProjectionTree {
    rpt_Node *root;
    int num_points_limit;
    int data_type_flag; //TODO 0 for data, 1 for data_tri
} RandomProjectionTree;

// returns midplane between two points at mid_plane
double *find_mid_vertical_plane(void *points, int num_points, int num_dimensions){

    if(num_points <= 2){
        printf("Error: no points to find midplane\n");
        exit(9);
    }

    Data d_array = (Data)points;

    int index_a = rand() % num_points;
    int index_b = rand() % num_points;
    while(index_a == index_b){
        index_b = rand() % num_points;
    }

    double *point1 = points[index_a].data_array;
    double *point2 = points[index_b].data_array;

    // we will return this
    double *hyperplane = (double *)malloc((num_dimensions + 1) * sizeof(double));
    //find vector of two points
    for (int i = 0; i < num_dimensions; ++i) {
        hyperplane[i] = point2[i] - point1[i];
    }
    //find midplane
    double mid_plane[num_dimensions];
    for (int i = 0; i < num_dimensions; ++i) {
        mid_plane[i] = (point1[i] + point2[i]) / 2.0;
    }
    //find a vector whose dot product with vector is 0, and has midplane as a point    
    double constant = 0;
    for (int i = 0; i < num_dimensions; ++i) {
        constant += mid_plane[i] * vector[i];
    }
    hyperplane[num_dimensions] = constant;
    
    return hyperplane;
}

double dot_product(double *vector1, double *vector2, int num_dimensions) {
    double result = 0.0;
    for (int i = 0; i < num_dimensions; ++i) {
        result += vector1[i] * vector2[i];
    }

    return result;
}
//find indices of points that belong in one of the two spaces in which the hyperplane divides the whole
//any indeces left out belong to the other space
void find_indices(double *projections, double constant int num_points, int *left_indices, int *right_indices, int *left_count, int *right_count) {
    int *right_indices = (int *)malloc(num_points * sizeof(int));
    int *left_indices = (int *)malloc(num_points * sizeof(int));
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
    
    left_indices = (int *)realloc(indices, left_count * sizeof(int));
    right_indices = (int *)realloc(indices, right_count * sizeof(int));
}


//Create a
rpt_Node* create_node(double *data, int *indices) {
    rpt_Node *node = (rpt_Node *)malloc(sizeof(rpt_Node));
    node->data = data;
    node->indices = indices;
    node->left = NULL;
    node->right = NULL;
    return node;
}


void build_tree_parallel(rpt_Node *node, void *points, int num_points, int num_dimensions, int current_depth, int num_point_limit) {
    //TODO add flag for data tri and handle then differently

    // this is all points, everything
    Data d_array = (Data)points;

    if (num_points < num_point_limit) {
        return;
    }

    // find perpendicular bisector hyperplane
    double *random_hyperplane = find_mid_vertical_plane(points,num_points,num_dimensions);
    
    // find projections of all points on this hyperplane(dot products)
    double *projections = (double *)malloc(num_points * sizeof(double));

    #pragma omp parallel for schedule(dynamic) // Use dynamic scheduling
    for (int i = 0; i < num_points; ++i) {
        projections[i] = dot_product(d_array[i].data_array, random_hyperplane, num_dimensions);
    }

    double constant = random_hyperplane[num_points];

    // // nth_element(projections, projections + median_index, projections + num_points);
    // median_projection = projections[median_index];

    // int *left_indices = find_indices(projections, median_projection, num_points);
    // int *right_indices = find_indices(projections, median_projection, num_points - left_count);

    int left_count = 0;
    int right_count = 0;

    int *left_indices;
    int *right_indices;
    find_indices(projections,consant, num_points, left_indices, right_indices, &left_count, &right_count);
    if(left_count == 0 || right_count == 0){
            //DO Stuff
    }

    free(projections);

    node->data = random_hyperplane;
    node->indices = left_indices;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            node->left = create_node(NULL, NULL);  // Left child
            build_tree_parallel(node->left, points(dataset), left point group,  left_count, num_dimensions, current_depth + 1, depth_limit);
        }

        #pragma omp section
        {
            node->right = create_node(NULL, NULL);  // Right child
            build_tree_parallel(node->right, points (dataset), right point group, right_count, num_dimensions, current_depth + 1, depth_limit);
        }
    }
}

void build_parallel(RandomProjectionTree *tree, double *points, int num_points, int num_dimensions) {
    tree->root = create_node(NULL, NULL);
    build_tree_parallel(tree->root, points, num_points, num_dimensions, 0, tree->depth_limit);
}

// int main() {
//     // ... (rest of the code remains the same)

//     RandomProjectionTree tree;
//     tree.depth_limit = 3;

//     double start_time = omp_get_wtime();
//     build_parallel(&tree, (double *)points, NUM_POINTS, NUM_DIMENSIONS);
//     double end_time = omp_get_wtime();

//     printf("Time taken: %f seconds\n", end_time - start_time);

//     // ... (rest of the code remains the same)

//     return 0;
// }
