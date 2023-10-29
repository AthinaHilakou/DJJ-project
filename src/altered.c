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
    int **myadjMatrix = (int **) createAdjMatrix(data_size, maxNeighbors);
    
    Heap *neighbors;
    neighbors = (Heap *) malloc(data_size * sizeof(Heap));

    // Sample K random neighbors for each node
    int* adjMatrix = (int*) malloc(data_size * sizeof(int));
    int neighbors_count;
    for(int j = 0; j < data_size; j++) {
        // get real and reverse neighbors
        adjMatrix = getNeighbors(myadjMatrix, j, data_size, &neighbors_count);
        // create heap from the neighbors & reverse neighbors
        float *weights = get_weights(adjMatrix, j, data, neighbors_count, dist_msr);
        neighbors[j] = heap_create(adjMatrix, neighbors_count, weights);
    }

    
    // Fina K real and ALL reverse neighbors for each node
    int **all_neighbors;
    all_neighbors = (int **) malloc(data_size * sizeof(int *));
    int all_neighbors_count;
    int sizes[data_size];
    for(int j = 0; j < data_size; j++) {
        // get real and reverse neighbors
        all_neighbors[j] = getAllNeighbors(myadjMatrix, j, data_size, &all_neighbors_count);
        sizes[j] = all_neighbors_count;
    }

    // main loop
    while(1){
        int update_counter = 0;
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
                    update_counter += heap_update(neighbors[i], neighbor_neighbor_index, weight);
                    }
                }
            }
            // break;
        }

    
    printf("done\n");
    
    

    return 0;

}