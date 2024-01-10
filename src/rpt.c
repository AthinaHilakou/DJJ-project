/* File random_projection_tree/rpr.c */
#include "../headers/rpt.h"

// returns midplane between two points at mid point
float *find_mid_vertical_plane(void *points, int num_points, int flag){

    if(num_points < 2){
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
void find_indices(float *projections, float constant, int num_points, int* indices, int **my_left_indices, int **my_right_indices, int *left_count, int *right_count){
    int *right_indices = (int *)malloc(num_points * sizeof(int));
    int *left_indices = (int *)malloc(num_points * sizeof(int));

    for (int i = 0; i < num_points; ++i) {
        if (projections[i] <= constant) {
            right_indices[(*right_count)++] = indices[i];
        } else{
            left_indices[(*left_count)++] = indices[i];
        }
    }

    while(*left_count == 0 || *right_count == 0){
        *left_count = 0;
        *right_count = 0;
        for(int i = 0; i < num_points; i++){
            if(rand() % 2 == 0){
                left_indices[(*left_count)++] = indices[i]; 
            } else{
                right_indices[(*right_count)++] = indices[i];
            }
        }
    }
    
    left_indices = (int *)realloc(left_indices, *left_count * sizeof(int));
    right_indices = (int *)realloc(right_indices, *right_count * sizeof(int));

    *my_left_indices = left_indices;
    *my_right_indices = right_indices;
}


//Create a node 
rpt_Node create_node(float *data, int *indices, int indices_size) {
    rpt_Node node = (rpt_Node)malloc(sizeof(my_rpt_node));
    node->data = data;
    
    node->indices = indices;
    node->left = NULL;
    node->right = NULL;
    node->indices_size = indices_size; 
    return node;
}


void build_tree_parallel(rpt_Node node, void *points, int *indices, int num_points, int flag, int num_point_limit, int thread_num) {
    //TODO add flag for data tri and handle then differently, replace num_dimensions with flag
    printf("num_points: %d\n", num_points);
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
    node->indices_size = num_points;
    node->num_points_limit = num_point_limit;

    // return condition, if we have reached the minimum limit

    if(num_points <= num_point_limit){
        return;
    }

    // return condition, if we have reached the minimum limit

    // printf("Indices: "  );
    // for(int i = 0; i < num_points; i++){
    //     printf("%d ", indices[i]);
    // }
    // printf("\n");
    // find perpendicular bisector hyperplane
    float *random_hyperplane = find_mid_vertical_plane(points,num_points,flag);
    if(random_hyperplane == NULL){
        printf("Error: mid plane failed\n");
        exit(9);
    }
    // find projections of all points on this hyperplane(dot products)
    float *projections = (float *)malloc(num_points * sizeof(float));
    if(projections == NULL){
        printf("Error: malloc failed\n");
        exit(9);
    }
    #pragma omp parallel for schedule(dynamic) // Use dynamic scheduling
    for (int i = 0; i < num_points; ++i) {
        projections[i] = dot_product(d_array[indices[i]].data_array, random_hyperplane, num_dimensions);
    }

    float constant;
    if(flag == 1){
        constant = random_hyperplane[3];
    }
    else{
        constant = random_hyperplane[100];
    }

    int left_count = 0;
    int right_count = 0;

    int *left_indices;
    int *right_indices;
    find_indices(projections,constant, num_points,indices,&left_indices, &right_indices, &left_count, &right_count);
    if(left_count == 1 || right_count == 1){
        left_count = 0;
        right_count = 0;
        for(int j = 0; j < num_points; j++){
            if(rand() % 2 == 0){
                left_indices[(left_count)++] = indices[j]; 
            } else{
                right_indices[(right_count)++] = indices[j];
            }
        }
    }
    //We stopped here--------------------------------------------------------------------------------------------
    free(projections);

    if(left_count < 3 || right_count < 3){
       return;
    }


    #pragma omp parallel sections
    {
        #pragma omp section
        {
            node->left = create_node(NULL, NULL,0);  // Left child, create it as empty fill it later
            build_tree_parallel(node->left, points, left_indices, left_count,flag, num_point_limit, thread_num);
        }

        #pragma omp section
        {
            node->right = create_node(NULL, NULL,0);  // Right child, create it as empty fill it later
            build_tree_parallel(node->right, points, right_indices, right_count, flag, num_point_limit, thread_num);
        }
    }

    
}

RandomProjectionTree rpt_tree_create(void *points, int num_points, int flag, int num_point_limit, int thread_num) {
    RandomProjectionTree tree = (RandomProjectionTree)malloc(sizeof(myRandomProjectionTree));
   

    int *indices = (int *)malloc(num_points * sizeof(int));
    #pragma omp parallel for schedule(dynamic) thread_count(thread_num) // Use dynamic scheduling
    for(int i = 0; i < num_points; i++){
        indices[i] = i;
    }
    tree->root = create_node(points,indices,num_points);
    tree->num_points_limit = num_point_limit;
    tree->data_type_flag = flag;

    int num_dimensions;
    if(flag == 0){
        num_dimensions = 100;
    } else{
        num_dimensions = 3;
    }
    build_tree_parallel(tree->root, points, indices, num_points, flag, num_point_limit, thread_num);




    return tree;
}


void rpt_destroy_helper(rpt_Node node){
    if(node != NULL){
        rpt_destroy_helper(node->left);
        rpt_destroy_helper(node->right);
        free(node->indices);
        free(node);
    }
}

int rpt_leaf_count_helper(rpt_Node node){
    if(node->left == NULL && node->right == NULL){
        return 1;
    }
    return rpt_leaf_count_helper(node->left) + rpt_leaf_count_helper(node->right);
}

int rpt_leaf_count(RandomProjectionTree tree){
    return rpt_leaf_count_helper(tree->root);
}


void rpt_tree_destroy(RandomProjectionTree tree){
    rpt_destroy_helper(tree->root);
    free(tree);
}

void rpt_get_indices_helper(rpt_Node node, int **indices, int *index, int *leaf_size){
    if(node == NULL){
        return;
    }
    if(node->left == NULL && node->right == NULL){
        indices[*index] = node->indices;
        leaf_size[*index] = node->indices_size;
        (*index)++;
        return;
    }
    rpt_get_indices_helper(node->left, indices, index, leaf_size);
    rpt_get_indices_helper(node->right, indices, index, leaf_size);
    
}

int **rpt_get_indices(RandomProjectionTree tree, int *leaf_count, int *leaf_size){
    *leaf_count = rpt_leaf_count(tree);
    int **indices = (int **)malloc((*leaf_count)* sizeof(int*));
    int index = 0;
    rpt_get_indices_helper(tree->root, indices, &index, leaf_size);
    return indices;
}



int **rpt_createAdjMatrix(void *points, int num_points, int flag, int num_point_limit, int thread_num){
    int **adj_matrix = (int **)malloc(num_points * sizeof(int *)); //adjacency matrix
    for(int i = 0; i < num_points; i++){ //initialize adj_matrix
        adj_matrix[i] = (int *)calloc(num_points, sizeof(int));
    }

    int leaf_count;
    RandomProjectionTree tree = rpt_tree_create(points, num_points, flag, num_point_limit/2, thread_num);
    int *leaf_size;
    int **indices = rpt_get_indices(tree, &leaf_count, leaf_size);
    for(int i = 0; i < leaf_count; i++){
        for(int j = 0; j < leaf_size[i]; j++){
            for(int k = j+1; k < leaf_size[i]; k++){
                adj_matrix[indices[i][j]][indices[i][k]] = 1;
                adj_matrix[indices[i][k]][indices[i][j]] = 1;
            }
            
        }
    }

    int reamaining = num_point_limit - num_point_limit/2;
    //add some random neighbors to complete the graph
    for(int i = 0; i < num_points; i++){
        for(int j = 0; j < reamaining; j++){
            int rand_index;
            do{
                rand_index=rand()%num_points;
            }while(adj_matrix[i][rand_index] == 1);
            adj_matrix[i][rand_index] = 1;
        }
    }

    //from tree, find neighbors + add some random neighbors
    // for()
    
    free(leaf_size);
    free(indices); //we pass imdices by reference, so we dont need to free it here
    rpt_tree_destroy(tree);

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
