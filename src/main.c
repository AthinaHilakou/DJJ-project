#include "../headers/graph.h"
#include "../headers/data.h"
#include "../headers/heap.h"
#include "../headers/data.h"
#include "../headers/my_data_tri.h"
#include "../headers/brute_force.h"
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <string.h>


void get_arguments(int argc, char** argv, int *maxNeighbors, float (*weight_fun)(void *, int, int, int), int *data_size, void* data, int* flag){

    if(argc < 4){
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag>\n");
        exit(1);
    }
    *maxNeighbors = atoi(argv[1]);
    *flag = atoi(argv[3]);
    
    if(*flag == 0){  // competition data case
        data = (Data)import_data(argv[2], data_size);
    } else if(*flag == 1){   // ascii data case
        data = (Data_tri)import_data_tri(argv[2], data_size);
    } else{ // false flag case
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag>\nso that flag = 0 || 1 \n");
        exit(1);
    }

    if(strcmp(argv[2], "manh") == 0){
        weight_fun = dist_manh;
    } else if(strcmp(argv[2], "eucl") == 0){
        weight_fun = dist_msr;
    }else{
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag>\nso that weight_fun = manh || eucl \n");
        exit(1);
    }

}

int main(int argc, char** argv){
    srand(time(NULL)); // seed random number generator
    clock_t start, end;
    double cpu_time_used;
    int maxNeighbors;
    float (*weight_fun)(void*, int, int, int);
    printf("Starting KNN aproximation %p\n", weight_fun);
    int data_size;
    start = clock();
    void* data;
    int flag;

    get_arguments(argc, argv, &maxNeighbors, weight_fun, &data_size, data, &flag);
    printf("After KNN aproximation %p\n", weight_fun);
    printf("Finished importing data in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);
    int **myadjMatrix = (int **)createAdjMatrix(data_size, maxNeighbors);
    Heap *neighbors;
    neighbors = (Heap *) malloc(data_size * sizeof(Heap));

    start = clock();

    // Sample K random neighbors for each node
    int* neighbor_indexes = (int*)malloc(data_size * sizeof(int));
    int neighbors_count;
    float *weights = (float *)malloc(maxNeighbors*sizeof(float));
    float **weights_array = (float **)malloc(data_size*sizeof(float *));
    for(int j = 0; j < data_size; j++) {
        weights_array[j] = (float *)malloc(data_size*sizeof(float));

        // get real and reverse neighbors
        getNeighbors(myadjMatrix, j, data_size, &neighbors_count, neighbor_indexes);
        // create heap from the neighbors & reverse neighbors
        get_weights(neighbor_indexes, j, data, neighbors_count, weight_fun, weights, flag);
        for(int i = 0; i < neighbors_count; i++){
            weights_array[j][neighbor_indexes[i]] = weights[i];
        }
        neighbors[j] = heap_create(neighbor_indexes, neighbors_count, weights);
    }

    for(int i = 0; i < data_size; i++){
        for(int j = 0; j < maxNeighbors; j++){
            weights_array[i][j] = -1;
        }
    }

    printf("Finished creating random neighbors in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);

    
    // Find K real and ALL reverse neighbors for each node
    int **all_neighbors;
    all_neighbors = (int **) malloc(data_size * sizeof(int *));
    for(int i = 0; i < data_size; i++) { //k plus the max number of reverse neighbors
        all_neighbors[i] = (int *) malloc((data_size + maxNeighbors)* sizeof(int));
    }
    int all_neighbors_count;
    int sizes[data_size];

    int outside = 0;
   
    start = clock();
    // main loop
    while(1){
        int update_counter = 0;
        for(int j = 0; j < data_size; j++) {
            // get real and reverse neighbors
            getAllNeighbors(myadjMatrix, j, data_size, &all_neighbors_count, all_neighbors[j]);
            sizes[j] = all_neighbors_count;
        }
        // for each node
        for(int i = 0; i < data_size; i++){
            // for each neighbor            
            for(int j = 0; j < sizes[i]; j++){
                int neighbor_index = all_neighbors[i][j];
                int neighbor_neighbor_size = sizes[neighbor_index];
                // for each neighbor of neighbor
                for(int k = 0; k < neighbor_neighbor_size; k++){
                    int neighbor_neighbor_index = all_neighbors[neighbor_index][k];
                    if(i == neighbor_neighbor_index){
                        continue;
                    }

                    float weight;
                    if(weights_array[i][neighbor_neighbor_index] - 1 != 0){
                        weight = weight_fun(data, i, neighbor_neighbor_index, flag);
                    } else{
                        weight = weights_array[i][neighbor_neighbor_index];
                    }
                    
                    int old_neighbor = index_from_heap(neighbors[i],0);
                    if(weight){ //ensure that neighbor's neighbor is not i itself AND that the edge exists
                        if(myadjMatrix[i][neighbor_neighbor_index] == 1){
                            continue;
                        }
                        if(heap_update(neighbors[i], neighbor_neighbor_index, weight) == true){
                            update_counter++;
                            removeEdge(myadjMatrix, i, old_neighbor);
                            addEdge(myadjMatrix, i, neighbor_neighbor_index);  
                        }
                    }
                }
            }
        }
        if(update_counter == 0){
            break;
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("KNN aproximation done in %3.2f seconds\n", cpu_time_used);
    // printf("KNN aproximation:\n");
    // printAdjMatrix(myadjMatrix, data_size);
    // printf("KNN brute force:\n");
    start = clock();
    printf("recall of graph is %1.3f\n", recall(myadjMatrix, 10, weight_fun, data, data_size, flag));
    printf("Brute force ended in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);

    for(int j = 0; j < data_size; j++) {
        // get real and reverse neighbors
        getAllNeighbors(myadjMatrix, j, data_size, &all_neighbors_count, all_neighbors[j]);
        sizes[j] = all_neighbors_count;
    }


    // search here
    search(myadjMatrix, dist_msr_ab, data, data_size, maxNeighbors, all_neighbors, sizes, flag);

    //Free resources
    for(int i = 0; i < data_size; i++){
        free(weights_array[i]);
    }
    free(weights_array);

    for(int i = 0; i < data_size; i++){
        heap_destroy(neighbors[i]);
    }
    free(neighbors);
    free(neighbor_indexes);
    for(int i = 0; i < data_size; i++){
        free(all_neighbors[i]);
    }
    free(all_neighbors);
    free(data);
    free(weights);
    freegraph(myadjMatrix, data_size);

    return 0;

}