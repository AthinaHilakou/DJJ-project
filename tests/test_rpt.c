#include "../headers/rpt.h"
#include "../headers/data.h"
#include "../headers/acutest.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void test_find_mid_vertical_plane(){
    srand ( time ( NULL));
    int num_points = 5;
    data points[num_points];
    for(int i =0; i < num_points; i++){
        for(int j = 0; j < 100; j++){
            points[i].data_array[j] = (double)rand()/RAND_MAX *2.0-1.0;
        }
    }
    float *mid_plane = find_mid_vertical_plane((void *)points, num_points, 0);
    
    TEST_CHECK(mid_plane != NULL);        

    free(mid_plane);
}

void test_dot_product(void){
    float a[3] = {1, 2, 3};
    float b[3] = {4, 5, 6};
    float c = dot_product(a, b, 3);
    TEST_CHECK(c == 32);

    float d[5] = {1, 2, 3, 4, 5};
    float e[5] = {6, 7, 8, 9, 10};
    float f = dot_product(d, e, 5);
    TEST_CHECK(f == 130);
}


void test_find_indices(){
    //Make 5 random points
    srand (time(NULL));
    int num_points = 5;
    data points[num_points];
    int indices[num_points];

    for(int i =0; i < num_points; i++){
        for(int j = 0; j < 100; j++){
            points[i].data_array[j] = (double)rand()/RAND_MAX *2.0-1.0;
        }
        indices[i] = i;
    }
    //find random hyperplane
    float *random_hyperplane = find_mid_vertical_plane(points,num_points,0);
    
    // find projections of all points on this hyperplane(dot products)
    float *projections = (float *)malloc(num_points * sizeof(float));



    #pragma omp parallel for schedule(dynamic) // Use dynamic scheduling
    for (int i = 0; i < num_points; ++i) {
        projections[i] = dot_product(points[i].data_array, random_hyperplane, 100);
    }

    float constant = random_hyperplane[100];

    int left_count = 0;
    int right_count = 0;

    int *left_indices;
    int *right_indices;
    find_indices(projections,constant, num_points,indices,&left_indices, &right_indices, &left_count, &right_count);
    free(projections);
    free(random_hyperplane);
    free(left_indices);
    free(right_indices);
    TEST_CHECK(left_count + right_count == num_points);
}

void test_rpt_tree_create(){
    srand (time(NULL));
    int num_points = 2000;
    data points[num_points];
    int indices[num_points];
    int num_limit = 10;

    for(int i =0; i < num_points; i++){
        for(int j = 0; j < 100; j++){
            points[i].data_array[j] = (double)rand()/RAND_MAX *2.0-1.0;
        }
        indices[i] = i;
    }
    //find random hyperplane
    float *random_hyperplane = find_mid_vertical_plane(points,num_points,0);
    
    // find projections of all points on this hyperplane(dot products)
    float *projections = (float *)malloc(num_points * sizeof(float));
    int flag = 0;
    int thread_num = 4;
    RandomProjectionTree tree = rpt_tree_create(points, num_points, flag, num_limit, thread_num);
    TEST_CHECK(tree != NULL);
    TEST_CHECK(tree->root != NULL);
    TEST_CHECK(tree->root->left != NULL);
    TEST_CHECK(tree->root->right != NULL);
    TEST_CHECK(tree->num_points_limit == num_limit);
    TEST_CHECK(tree->data_type_flag == flag);

    int leaf_count = rpt_leaf_count(tree);
    int myleaf_count = 0;
    int *leaf_size = malloc(sizeof(int)*(leaf_count));
    int **leaf_indices = rpt_get_indices(tree, &leaf_count, leaf_size);
    for(int i = 0; i < leaf_count; i++){
        myleaf_count += leaf_size[i];
    }
    TEST_CHECK(myleaf_count == num_points);
    free(leaf_indices);
    free(leaf_size);
    free(projections);
    free(random_hyperplane);
    rpt_tree_destroy(tree);
}


void test_rpt_createAdjMatrix(){
    //Make 5 random points
    srand (time(NULL));
    int num_points = 100;
    data points[num_points];
    int indices[num_points];
    int num_point_limit = 10; 

    for(int i =0; i < num_points; i++){
        for(int j = 0; j < 100; j++){
            points[i].data_array[j] = (double)rand()/RAND_MAX *2.0-1.0;
        }
        indices[i] = i;
    }
    int **adj_matrix = rpt_createAdjMatrix(points, num_points, 0,num_point_limit, 4);
    int count = 0;
    TEST_CHECK(adj_matrix != NULL);
    for(int i = 0; i < num_points; i++){
        count = 0;
        for(int j = 0; j < num_points; j++){
            if(adj_matrix[i][j] == 1){
                count++;
            }
        }
        TEST_CHECK(count == num_point_limit);
    }

}

TEST_LIST = {
    {"test_find_mid_vertical_plane", test_find_mid_vertical_plane},
    {"test_dot_product", test_dot_product},
    {"test_find_indices", test_find_indices},
    {"test_rpt_tree_create", test_rpt_tree_create},
    {"test_rpt_createAdjMatrix", test_rpt_createAdjMatrix},
    {NULL, NULL}
};

