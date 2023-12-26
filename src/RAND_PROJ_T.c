#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NUM_DIMENSIONS 2
#define NUM_POINTS 100

typedef struct rpt_Node {
    double *data;
    int *indices;
    struct Node *left;
    struct Node *right;
} rpt_Node;

typedef struct RandomProjectionTree {
    rpt_Node *root;
    int depth_limit;
} RandomProjectionTree;

// ... (rest of the code remains the same)

double* generate_random_hyperplane(int num_dimensions) {
    double *random_hyperplane = (double *)malloc(num_dimensions * sizeof(double));
    for (int i = 0; i < num_dimensions; ++i) {
        random_hyperplane[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;  // Random values between -1 and 1
    }
    return random_hyperplane;
}

double dot_product(double *vector1, double *vector2, int num_dimensions) {
    double result = 0.0;
    for (int i = 0; i < num_dimensions; ++i) {
        result += vector1[i] * vector2[i];
    }
    return result;
}

int* find_indices(double *projections, double median_projection, int num_points) {
    int *indices = (int *)malloc(num_points * sizeof(int));
    int count = 0;
    for (int i = 0; i < num_points; ++i) {
        if (projections[i] <= median_projection) {
            indices[count++] = i;
        }
    }
    indices = (int *)realloc(indices, count * sizeof(int));
    return indices;
}

rpt_Node* create_node(double *data, int *indices) {
    rpt_Node *node = (rpt_Node *)malloc(sizeof(rpt_Node));
    node->data = data;
    node->indices = indices;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void build_tree_parallel(rpt_Node *node, double *points, int num_points, int num_dimensions, int current_depth, int depth_limit) {
    if (num_points == 0 || current_depth == depth_limit) {
        return;
    }

    double *random_hyperplane = generate_random_hyperplane(num_dimensions);

    double *projections = (double *)malloc(num_points * sizeof(double));

    #pragma omp parallel for schedule(dynamic) // Use dynamic scheduling
    for (int i = 0; i < num_points; ++i) {
        projections[i] = dot_product(&points[i * num_dimensions], random_hyperplane, num_dimensions);
    }

    double median_projection;
    int median_index = num_points / 2;
    nth_element(projections, projections + median_index, projections + num_points);
    median_projection = projections[median_index];

    int *left_indices = find_indices(projections, median_projection, num_points);
    int *right_indices = find_indices(projections, median_projection, num_points - left_count);

    free(projections);

    node->data = random_hyperplane;
    node->indices = left_indices;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            node->left = create_node(NULL, NULL);  // Left child
            build_tree_parallel(node->left, points, left_count, num_dimensions, current_depth + 1, depth_limit);
        }

        #pragma omp section
        {
            node->right = create_node(NULL, NULL);  // Right child
            build_tree_parallel(node->right, points + left_count * num_dimensions, right_count, num_dimensions, current_depth + 1, depth_limit);
        }
    }
}

void build_parallel(RandomProjectionTree *tree, double *points, int num_points, int num_dimensions) {
    tree->root = create_node(NULL, NULL);
    build_tree_parallel(tree->root, points, num_points, num_dimensions, 0, tree->depth_limit);
}

int main() {
    // ... (rest of the code remains the same)

    RandomProjectionTree tree;
    tree.depth_limit = 3;

    double start_time = omp_get_wtime();
    build_parallel(&tree, (double *)points, NUM_POINTS, NUM_DIMENSIONS);
    double end_time = omp_get_wtime();

    printf("Time taken: %f seconds\n", end_time - start_time);

    // ... (rest of the code remains the same)

    return 0;
}
