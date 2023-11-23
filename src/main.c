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
    srand(time(NULL)); // seed random number generator
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
    float *weights = (float *)malloc(maxNeighbors*sizeof(float));
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
        mapify(map_all_neigbors[j], neighbor_indexes, weights, neighbors_count);
    }

    printf("Finished creating random neighbors in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);


    // Find K real and ALL reverse neighbors for each node
    // all_neighbors = (int **) malloc(data_size * sizeof(int *));
    // for(int i = 0; i < data_size; i++) { //k plus the max number of reverse neighbors
    //     all_neighbors[i] = (int *) malloc((data_size + maxNeighbors)* sizeof(int));
    // }


    start = clock();
    // main loop
        // for(int j = 0; j < data_size; j++) {
        //     // get real and reverse neighbors
        //     getAllNeighbors(myadjMatrix, j, data_size, &all_neighbors_count, all_neighbors[j]);
        //     sizes[j] = all_neighbors_count;
        // }
    int **all_neighbors = (int **) malloc(data_size * sizeof(int *));
    int sizes[data_size];
    int out_counter = 0;
    while(1){
        printf("Out loop\n\n\n");
        int update_counter = 0;
        for(int i = 0; i < data_size; i++){
            all_neighbors[i] = map_to_array(map_all_neigbors[i], &sizes[i]); 
            if( i < 3){
                printf("Neibhors %d\n", i);
                map_print(map_all_neigbors[i]);
            }
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
                    float weight;
                   
                    if(i == neighbor1 || i == neighbor2 || neighbor1 == neighbor2){
                        continue;
                    }
                
                    if(weights_array[neighbor1][neighbor2] - 1 != 0){
                        weight = weight_fun(data,neighbor1,neighbor2, flag);
                        weights_array[neighbor1][neighbor2] = weight;
                    } else{
                        weight = weights_array[neighbor1][neighbor2];
                    }

                    int ret;
                    int old_neighbor1 = index_from_heap(neighbors[neighbor1],0);
                    int old_neighbor2 = index_from_heap(neighbors[neighbor2],0);
                    if(update_counter < 2){
                        if(i < 2){
                            printf("------------- heap of %d\n", neighbor1);
                            print_heap(neighbors[neighbor1]);
                            printf("------------- map of %d\n", neighbor1);
                            map_print(map_all_neigbors[neighbor1]);
                        }

                    }
                    if(heap_update(neighbors[neighbor1],neighbor2, weight) == true){
                        printf("heap neihbor1 = %d, removed %d, added %d\n", neighbor1, old_neighbor1, neighbor2);
                        update_counter++;
                        ret = map_update(map_all_neigbors[neighbor1], neighbor2, weight,old_neighbor1);
                        if(ret){
                            printf("map update neihbor1 = %d, removed %d, added %d\n\n\n", neighbor1, old_neighbor1, neighbor2);
                        }
                    }
                    if(heap_update(neighbors[neighbor2],neighbor1, weight) == true){
                        update_counter++;
                        ret = map_update(map_all_neigbors[neighbor2],neighbor1, weight, old_neighbor2);
                        if(ret){
                            printf("neighbor2 = %d, removed %d, added %d\n", neighbor2, old_neighbor2, neighbor1);
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
    // printf("KNN aproximation:\n");
    // printAdjMatrix(myadjMatrix, data_size);
    // printf("KNN brute force:\n");
    start = clock();
    printf("recall of graph is %1.3f\n", recall(myadjMatrix, 10, weight_fun, data, data_size, flag));
    printf("Brute force ended in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);

     for(int i = 0; i < data_size; i++){
        all_neighbors[i] = map_to_array(map_all_neigbors[i], sizes + i);
    }
    // for(int j = 0; j < data_size; j++) {
    //     // get real and reverse neighbors
    //     int all_neighbors_count;
    //     getAllNeighbors(myadjMatrix, j, data_size, &all_neighbors_count, all_neighbors[j]);
    //     sizes[j] = all_neighbors_count;
    // }


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
