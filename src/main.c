#include "../headers/graph.h"
#include "../headers/data.h"
#include "../headers/heap.h"
#include "../headers/map.h"
#include "../headers/data.h"
#include "../headers/my_data_tri.h"
#include "../headers/brute_force.h"
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <string.h>

void get_arguments(int argc, char** argv, int *maxNeighbors, float (**weight_fun)(void *, int, int, int), int *data_size, void** data, int* flag);



int main(int argc, char** argv){
    srand(time(NULL)* getpid()); // seed random number generator
    clock_t start, end;
    double cpu_time_used;
    int maxNeighbors;
    float (*weight_fun)(void*, int, int, int);
    printf("Starting KNN aproximation\n");
    int data_size;
    start = clock();
    void** data_p = malloc(sizeof(void *));
    int flag;

    get_arguments(argc, argv, &maxNeighbors, &weight_fun, &data_size, data_p, &flag);
    void* data = *data_p;
    printf("Finished importing data in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);
    int **myadjMatrix = (int **)createAdjMatrix(data_size, maxNeighbors);
    Heap *neighbors;
    neighbors = (Heap *) malloc(data_size * sizeof(Heap));

    start = clock();
    printf("Starting creating random neighbors\n");
    // Sample K random neighbors for each node
    int neighbors_count;
    int* neighbor_indexes = (int*)malloc(data_size * sizeof(int));
    float *weights = (float *)malloc(data_size*sizeof(float));
    float **weights_array = (float **)malloc(data_size*sizeof(float *));
    Map *map_all_neigbors = (Map *)malloc(data_size*sizeof(Map));

    //create empty maps for all neighbors save
    for(int i = 0; i < data_size; i++){
        map_all_neigbors[i] = map_init(4*maxNeighbors);
    }
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
        getAllNeighbors(myadjMatrix, j, data_size, &neighbors_count, neighbor_indexes);
        get_weights(neighbor_indexes, j, data, neighbors_count, weight_fun, weights, flag);
        mapify(map_all_neigbors[j], neighbor_indexes, weights, neighbors_count);
    }

    printf("Finished creating random neighbors in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);

    // Find K real and ALL reverse neighbors for each node
    // all_neighbors = (int **) malloc(data_size * sizeof(int *));
    // for(int i = 0; i < data_size; i++) { //k plus the max number of reverse neighbors
    //     all_neighbors[i] = (int *) malloc((data_size + maxNeighbors)* sizeof(int));
    // }


    start = clock();

    int **all_neighbors = (int **) malloc(data_size * sizeof(int *));
    int sizes[data_size];
    int out_counter = 0;
    // main loop

    while(1){
        int update_counter = 0;
        // printAdjMatrix(myadjMatrix, data_size);
        for(int i = 0; i < data_size; i++){
            all_neighbors[i] = map_to_array(map_all_neigbors[i], &sizes[i]); 
        }
        // for each node
        for(int i = 0; i < data_size; i++){
            // printf("Checking %d point\n", i);
            // for each neighbor
            int all_neighbors_count;
            for(int j = 0; j < sizes[i]; j++){
                int neighbor1 = all_neighbors[i][j];
                // for all neighbor pairs
                for(int k = j + 1; k < sizes[i]; k++){
                    int neighbor2 = all_neighbors[i][k];
                   
                    if(i == neighbor1 || i == neighbor2 || neighbor1 == neighbor2){
                        continue;
                    }
                
                    float weight;
                    if(weights_array[neighbor1][neighbor2] - 1 != 0){
                        weight = weight_fun(data,neighbor1,neighbor2, flag);
                        weights_array[neighbor1][neighbor2] = weight;
                    } else{
                        weight = weights_array[neighbor1][neighbor2];
                    }

                    int ret;
                    int old_neighbor1 = heap_find_max(neighbors[neighbor1]);
                    int old_neighbor2 = heap_find_max(neighbors[neighbor2]);
                    // if neighbor2 is not in neighbor1's neighbors
                    if(myadjMatrix[neighbor1][neighbor2] == 0){
                        if(heap_update(neighbors[neighbor1],neighbor2, weight) == true){
                            // print_heap(neighbors[neighbor1]);
                            update_counter++;
                            removeEdge(myadjMatrix, neighbor1, old_neighbor1);
                            addEdge(myadjMatrix, neighbor1, neighbor2);
                            // if neighbor1 is not in old_neighbor1's neighbors
                            if(myadjMatrix[old_neighbor1][neighbor1] == 0){
                                map_remove(map_all_neigbors[old_neighbor1], neighbor1);
                                map_remove(map_all_neigbors[neighbor1], old_neighbor1);
                            }
                            // if neighbor2 is not in neighbor1's neighbors
                            map_add(map_all_neigbors[neighbor2], neighbor1, weight);
                            map_add(map_all_neigbors[neighbor1], neighbor2, weight);
                        }
                    }

                    if (myadjMatrix[neighbor2][neighbor1] == 0){
                        if(heap_update(neighbors[neighbor2],neighbor1, weight) == true){
                            update_counter++;
                            removeEdge(myadjMatrix, neighbor2, old_neighbor2);
                            addEdge(myadjMatrix, neighbor2, neighbor1);
                         
                            // if neighbor2 is not in old_neighbor2's neighbors
                            if(myadjMatrix[old_neighbor2][neighbor2] == 0 && myadjMatrix[neighbor2][old_neighbor2] == 0){
                                map_remove(map_all_neigbors[old_neighbor2], neighbor2);
                                map_remove(map_all_neigbors[neighbor2], old_neighbor2);
                            }
                            // if neighbor1 is not in neighbor2's neighbors
                            map_add(map_all_neigbors[neighbor1], neighbor2, weight);
                            map_add(map_all_neigbors[neighbor2], neighbor1, weight);
                            
                        }
                    }
                }
            }
        }
        if(update_counter == 0){
            break;
        }
        // free(all_neighbors);
    }

    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("KNN aproximation done in %3.2f seconds\n", cpu_time_used);
    start = clock();
    printf("recall of graph is %1.3f\n", recall(myadjMatrix, maxNeighbors, weight_fun, data, data_size, flag));
    printf("Brute force ended in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);

     for(int i = 0; i < data_size; i++){
        all_neighbors[i] = map_to_array(map_all_neigbors[i], sizes + i);
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



void get_arguments(int argc, char** argv, int *maxNeighbors, float (**weight_fun)(void *, int, int, int), int *data_size, void** data, int* flag){
    if(argc < 4){
        printf("Usage: ./main <maxNeighbors> <file_name> <weight_function> <flag>\n");
        exit(1);
    }
    *maxNeighbors = atoi(argv[1]);
    *flag = atoi(argv[3]);

    if(*flag == 0){  // competition data case
        *data = (Data)import_data(argv[2], data_size);
    } else if(*flag == 1){   // ascii data case
        *data = (Data_tri)import_data_tri(argv[2], data_size);
    } else{ // false flag case
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag>\nso that flag = 0 || 1 \n");
        exit(1);
    }
    if(strcmp(argv[3], "manh") == 0){
        *weight_fun = dist_manh;
    } else if(strcmp(argv[3], "eucl") == 0){
        *weight_fun = dist_msr;
    }else{
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag>\nso that weight_fun = manh || eucl \n");
        exit(1);
    }

}
