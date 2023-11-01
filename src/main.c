#include "../headers/graph.h"
#include "../headers/data.h"
#include "../headers/heap.h"
#include "../headers/data.h"
#include "../headers/brute_force.h"
#include <stdlib.h>
#include <sys/mman.h>

int main() {
    srand(time(NULL)); // seed random number generator

    int maxNeighbors = 10;
    int data_size;
    Data data = import_data("datasets/given/00000020.bin", &data_size);

    int **myadjMatrix = (int **)createAdjMatrix(data_size, maxNeighbors);
    // printAdjMatrix(myadjMatrix, data_size);
    Heap *neighbors;
    neighbors = (Heap *) malloc(data_size * sizeof(Heap));

    // Sample K random neighbors for each node
    int* neighbor_indexes = (int*)malloc(data_size * sizeof(int));
    int neighbors_count;
    float *weights = (float *)malloc(maxNeighbors*sizeof(float));
    for(int j = 0; j < data_size; j++) {
        // get real and reverse neighbors
        getNeighbors(myadjMatrix, j, data_size, &neighbors_count, neighbor_indexes);
        // create heap from the neighbors & reverse neighbors
        get_weights(neighbor_indexes, j, data, neighbors_count, dist_msr, weights);
        neighbors[j] = heap_create(neighbor_indexes, neighbors_count, weights);
    }

    
    // Find K real and ALL reverse neighbors for each node
    int **all_neighbors;
    all_neighbors = (int **) malloc(data_size * sizeof(int *));
    for(int i = 0; i < data_size; i++) { //k plus the max number of reverse neighbors
        all_neighbors[i] = (int *) malloc((data_size + maxNeighbors)* sizeof(int));
    }
    int all_neighbors_count;
    int sizes[data_size];
   
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
                    float weight = dist_msr(data, i, neighbor_neighbor_index);
                    int old_neighbor = index_from_heap(neighbors[i],0);
                    if(weight){ //ensure that neighbor's neighbor is not i itself AND that the edge exists
                        if(myadjMatrix[i][neighbor_neighbor_index] == 1){
                            continue;
                        }
                        if(i == 0){

                        printf("Before update %d:\n", i);
                        print_heap(neighbors[i]);
                        }
                        if(heap_update(neighbors[i], neighbor_neighbor_index, weight) == true){
                            printf("update edge %d %d->%d\n", i, old_neighbor, neighbor_neighbor_index);
                            update_counter++;
                            removeEdge(myadjMatrix, i, old_neighbor);
                            addEdge(myadjMatrix, i, neighbor_neighbor_index);  
                        }
                        if(i == 0){
                        printf("After update %d:\n", i);
                        print_heap(neighbors[i]);
                        }

                    }
                }
            }
            // break;
        }
        if(update_counter == 0){
            break;
        }
    }
    // printf("KNN aproximation:\n");
    // printAdjMatrix(myadjMatrix, data_size);
    // printf("KNN brute force:\n");
    printf("recall of graph is %f\n", recall(myadjMatrix, 10, dist_msr, data, data_size));

    printf("KNN weights:\n");



    for(int i = 0; i < data_size; i++){
        printf("%2d: ", i);
        for(int j = 0; j < data_size; j++){
            if(myadjMatrix[i][j] == 1){
                printf("%d %1.2f, ", j, dist_msr(data, i, j));
            }
        }
        printf("\n");
    }

    //Free resources
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