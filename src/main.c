#include "../headers/graph.h"
#include "../headers/data_tri.h"
#include "../headers/heap.h"
#include "../headers/data.h"
#include <stdlib.h>
#include <sys/mman.h>

int main() {
    int numVertices = 20;
    int maxNeighbors = 10;

    Data_tri *data = (Data_tri *) import_data_tri("datasets/ascii/5k.orig.tri.ascii");
    
    Heap *neighbors;
    neighbors = (Heap *) malloc(numVertices * sizeof(Heap));
    srand(time(NULL)); // seed random number generator

    int **myadjMatrix = (int **) createAdjMatrix(numVertices, maxNeighbors);

    // Sample
    int* adjMatrix = (int*) malloc(numVertices * sizeof(int));
    int neighbors_count;
    for(int j = 0; j < numVertices; j++) {
        // get real and reverse neighbors
        adjMatrix = getAllNeighbors(myadjMatrix, j, numVertices, &neighbors_count);
        // create heap from the neighbors & reverse neighbors
        neighbors[j] = heap_create(adjMatrix,(void *) &data[j], dist_msr, neighbors_count, neighbors_count, sizeof(Data_tri));
    }



    while(1){
        int c = 0;
        // for each node
        for(int i = 0; i < numVertices; i++){
            // for each neighbor
            for(int j = get_heap_size(neighbors[i]); j > 0; j--){
                // for each neighbor of neighbor
                for(int k = get_heap_size(neighbors[j]); k > 0; k--){
                    // if neighbor of neighbor is not a neighbor of node
                    if(heap_find_max(neighbors[i]) != heap_find_max(neighbors[j])){
                        // add neighbor of neighbor to node
                        heap_insert(neighbors[i], heap_find_max(neighbors[j]));
                    }
                }
            }
        }
        break;

    }
    printf("done\n");
    
    

    return 0;

}