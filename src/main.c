#include "../headers/graph.h"
#include "../headers/data.h"
#include "../headers/heap.h"
#include "../headers/data.h"
#include <stdlib.h>
#include <sys/mman.h>

int main() {
    srand(time(NULL)); // seed random number generator
    int numVertices = 20;
    int maxNeighbors = 5;
    int data_size;
    Data data = import_data("datasets/given/00000020.bin", &data_size);
    int **myadjMatrix = (int **) createAdjMatrix(numVertices, maxNeighbors);

    Heap *neighbors;
    neighbors = (Heap *) malloc(numVertices * sizeof(Heap));

    // Sample K random neighbors for each node
    int* adjMatrix = (int*) malloc(numVertices * sizeof(int));
    int neighbors_count;
    for(int j = 0; j < numVertices; j++) {
        // get real and reverse neighbors
        adjMatrix = getNeighbors(myadjMatrix, j, numVertices, &neighbors_count);
        // create heap from the neighbors & reverse neighbors
        printNeighbors(adjMatrix, neighbors_count, j);
        int *weights = get_weights(adjMatrix, j, data, neighbors_count, dist_msr);
        neighbors[j] = heap_create(adjMatrix, j, neighbors_count, weights);
    }

    // Fina K real and ALL reverse neighbors for each node
    Heap *all_neighbors;
    all_neighbors = (Heap *) malloc(numVertices * sizeof(Heap));
    int all_neighbors_count;
    for(int j = 0; j < numVertices; j++) {
        // get real and reverse neighbors
        adjMatrix = getAllNeighbors(myadjMatrix, j, numVertices, &all_neighbors_count);
        // create heap from the neighbors & reverse neighbors
        int *weights = get_weights(adjMatrix, j, data, neighbors_count, dist_msr);
        all_neighbors[j] = heap_create(adjMatrix, j, all_neighbors_count, weights);
    }

    // main loop
    while(1){
        int update_counter = 0;
        // for each node

        for(int i = 0; i < numVertices; i++){
            // for each neighbor
            int all_neighbors_size = get_heap_size(all_neighbors[i]);
            for(int j = 0; j < all_neighbors_size; j++){
                // for each neighbor of neighbor
                int neighbor_index = index_from_heap(all_neighbors[i], j);
                int neighbor_neighbors_size = get_heap_size(all_neighbors[neighbor_index]);
                for(int k = 0; k < neighbor_neighbors_size; k++){
                    int neighbor_neighbor_index = index_from_heap(all_neighbors[neighbor_index], k);
                    int weight = dist_msr(data, i, neighbor_neighbor_index);

                    update_counter += heap_update(neighbors[i], neighbor_neighbor_index, weight);


                    }
                }
            }
            // break;
        }

    
    printf("done\n");
    
    

    return 0;

}