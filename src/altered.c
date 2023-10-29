#include "../headers/graph.h"
#include "../headers/data.h"
#include "../headers/heap.h"
#include "../headers/data.h"
#include <stdlib.h>
#include <sys/mman.h>

int altered() {
    srand(time(NULL)); // seed random number generator

    int maxNeighbors = 10;
    int data_size;
    Data data = import_data("datasets/given/00000020.bin", &data_size);

    int **myadjMatrix = (int **)createAdjMatrix(data_size, maxNeighbors);
    
    Heap *neighbors;
    neighbors = (Heap *) malloc(data_size * sizeof(Heap));

    // Sample K random neighbors for each node
    int* neighbor_indexes = (int*)malloc(data_size * sizeof(int));
    int neighbors_count;
    for(int j = 0; j < data_size; j++) {
        // get real and reverse neighbors
        getNeighbors(myadjMatrix, j, data_size, &neighbors_count, neighbor_indexes);
        // create heap from the neighbors & reverse neighbors
        float *weights = get_weights(neighbor_indexes, j, data, neighbors_count, dist_msr);
        neighbors[j] = heap_create(neighbor_indexes, neighbors_count, weights);
    }

    
    // Fina K real and ALL reverse neighbors for each node
    int **all_neighbors;
    all_neighbors = (int **) malloc(data_size * sizeof(int *));
    for(int i = 0; i < data_size; i++) {
        all_neighbors[i] = (int *) malloc(data_size * sizeof(int));
    }
    int all_neighbors_count;
    int sizes[data_size];
    for(int j = 0; j < data_size; j++) {
        // get real and reverse neighbors
        getAllNeighbors(myadjMatrix, j, data_size, &all_neighbors_count, all_neighbors[j]);
        sizes[j] = all_neighbors_count;
    }

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
                // for each neighbor of neighbor
                int neighbor_index = all_neighbors[i][j];
                int neighbor_neighbor_size = sizes[neighbor_index];
                for(int k = 0; k < neighbor_neighbor_size; k++){
                    int neighbor_neighbor_index = all_neighbors[neighbor_index][k];
                    float weight = dist_msr(data, i, neighbor_neighbor_index);
                    int old_neighbor = index_from_heap(neighbors[i],0);
                    if(heap_update(neighbors[i], neighbor_neighbor_index, weight)){
                       update_counter++;
                       addEdge(myadjMatrix, i, neighbor_neighbor_index);
                       removeEdge(myadjMatrix, i, old_neighbor);
                    }
                }
            }
            // break;
        }
        if(update_counter == 0){
            break;
        }
    }
    printf("done\n");
    
    

    return 0;

}