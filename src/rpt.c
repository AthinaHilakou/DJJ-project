/* File random_projection_tree/rpr.c */
#include "../headers/rpt.h"

// returns midplane between two points at mid_plane
float *find_mid_vertical_plane(void *points, int num_points, int flag){

    if(num_points <= 2){
        printf("Error: no points to find midplane\n");
        exit(9);
    }
    int num_dimensions = 0;
    Data d_array = NULL;
    Data_tri d_array_tri = NULL;
    if(flag == 0){
        d_array = (Data)points;
        num_dimensions = 100;
    } else{
        d_array_tri = (Data_tri)points;
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


void build_tree_parallel(rpt_Node *node, void *points, int *indices, int num_points, int flag, int num_point_limit, int thread_num) {
    //TODO add flag for data tri and handle then differently, replace num_dimensions with flag

    // this is all points, everything
    node->data = points;
    int num_dimensions;
    Data d_array = NULL;
    Data_tri d_array_tri = NULL;
    if(flag == 0){
        d_array = (Data)node->data;
        num_dimensions = 100;
    } else{
        d_array_tri = (Data_tri)node->data;
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

void build_parallel(RandomProjectionTree *tree, void *points, int num_points, int flag, int num_point_limit, int thread_num) {
    tree->root = create_node(NULL, NULL);
    int *indices = (int *)malloc(num_points * sizeof(int));
    #pragma omp parallel for schedule(dynamic) thread_count(thread_num) // Use dynamic scheduling
    for(int i = 0; i < num_points; i++){
        indices[i] = i;
    }
    int num_dimensions;
    if(flag == 0){
        num_dimensions = 100;
    } else{
        num_dimensions = 3;
    }
    build_tree_parallel(tree->root, points, indices, num_points, num_dimensions, num_point_limit);
}


int **rpt_createAdjMatrix(void *points, int num_points, int flag, int num_point_limit, int thread_num){
    //build tree
    //from tree, find neighbors + add some randomness
}



// int main(int argc, char** argv){

//     float *norms;

//     srand(348); // seed random number generator
//     int maxNeighbors;
//     float (*weight_fun)(void*, int, int, int);
//     printf("Starting KNN aproximation\n");
//     int data_size;
//     void** data_p = malloc(sizeof(void *));
//     int flag;
//     double delta = 0.0001; //default value of delta parameter
//     double sampling_rate = 0.4; //default value of sampling_rate parameter

//     get_arguments_rpt(argc, argv, &maxNeighbors, &weight_fun, &data_size, data_p, &flag, &delta, &sampling_rate);
//     void* data = *data_p;

//     RandomProjectionTree tree;

//     float start_time = omp_get_wtime();
//     // build_parallel(&tree, data, NUM_POINTS, NUM_DIMENSIONS);
//     float end_time = omp_get_wtime();

//     printf("Time taken: %f seconds\n", end_time - start_time);

//     // ... (rest of the code remains the same)

//     return 0;
// }
