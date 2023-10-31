#include "../headers/graph.h"
#include "../headers/data.h"
#include "../headers/heap.h"
#include "../headers/data.h"
#include <stdlib.h>
#include <sys/mman.h>

int main() {
    srand(time(NULL)); // seed random number generator

    int maxNeighbors = 10;
    int data_size;
    Data data = import_data("datasets/given/00000020.bin", &data_size);

    int **myadjMatrix = (int **)createAdjMatrix(data_size, maxNeighbors);
    printAdjMatrix(myadjMatrix, data_size);
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
        printf("loop\n");
        int update_counter = 0;
        for(int j = 0; j < data_size; j++) {
            // get real and reverse neighbors
            getAllNeighbors(myadjMatrix, j, data_size, &all_neighbors_count, all_neighbors[j]);
            sizes[j] = all_neighbors_count;
            //for(int i = 0; i < all_neighbors_count; i++){
                //printf("%d ", all_neighbors[j][i]);
            //}
            //printf("\n");
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
                    if(i == neighbor_neighbor_index){
                        continue;
                    }
                    //printf("{i = %d , n = %d, nn index = %d}\n",i, neighbor_index, neighbor_neighbor_index);
                    float weight = dist_msr(data, i, neighbor_neighbor_index);
                    int old_neighbor = index_from_heap(neighbors[i],0);
                    if(weight){ //ensure that neighbor's neighbor is not i itself AND that the edge exists
                        if(heap_update(neighbors[i], neighbor_neighbor_index, weight) == true){
                            update_counter++;
                            printf("i = %d, old top = %d, new top = %d\n", i, old_neighbor, index_from_heap(neighbors[i],0));
                            //printf("i, nn weight = %f,i = %d, old_top = %d, nn index = %d\n", weight, i, old_neighbor, neighbor_neighbor_index);
                            removeEdge(myadjMatrix, i, old_neighbor);
                            addEdge(myadjMatrix, i, neighbor_neighbor_index);  
                        }
                    }
                }
            }
            // break;
        }
        printf("update_counter: %d\n", update_counter);
        if(update_counter == 0){
            break;
        }
    }
    printf("done\n");
    printAdjMatrix(myadjMatrix, data_size);
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