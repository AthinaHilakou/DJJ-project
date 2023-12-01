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

void get_arguments(int argc, char** argv, int *maxNeighbors, float (**weight_fun)(void *, int, int, int), int *data_size, void** data, int* flag, double* delta, double* sampling_rate);
void update_insert_flag(int *);


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
    double delta = 0.0001; //default value of delta parameter
    double sampling_rate = 0.4; //default value of sampling_rate parameter

    get_arguments(argc, argv, &maxNeighbors, &weight_fun, &data_size, data_p, &flag, &delta, &sampling_rate);
    void* data = *data_p;
    printf("Finished importing data in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);
    int **myadjMatrix = (int **)createAdjMatrix(data_size, maxNeighbors);
    int **insert_flags; //flag matrix for inceremental search
    insert_flags = (int **) malloc(data_size * sizeof(int *));
    for(int i = 0; i < data_size; i++) { //k plus the max number of reverse neighbors
        insert_flags[i] = (int *) malloc((data_size)* sizeof(int));
    }
    for(int i = 0; i < data_size; i++){ //initialise insert_flags
        for(int j = 0; j < data_size; j++){
            insert_flags[i][j] = -1;
        }
    }

    Heap *neighbors;
    neighbors = (Heap *) malloc(data_size * sizeof(Heap));

    start = clock();
    // Sample K random neighbors for each node
    int neighbors_count;
    int* neighbor_indexes = (int*)malloc(data_size * sizeof(int));
    float *weights = (float *)malloc(data_size*sizeof(float));
    float **weights_array = (float **)malloc(data_size*sizeof(float *));
    Map *reverse_neighbors = (Map *)malloc(data_size*sizeof(Map));

    //create empty maps for all neighbors save
    for(int i = 0; i < data_size; i++){
        reverse_neighbors[i] = map_init(4*maxNeighbors);
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
        for(int i = 0; i < neighbors_count; i++){
            update_insert_flag(&insert_flags[j][neighbor_indexes[i]]);
        }
        getReverseNeighbors(myadjMatrix, j, data_size, &neighbors_count, neighbor_indexes);
        get_weights(neighbor_indexes, j, data, neighbors_count, weight_fun, weights, flag);
        mapify(reverse_neighbors[j], neighbor_indexes, weights, neighbors_count);
        for(int i = 0; i < neighbors_count; i++){
            if(insert_flags[j][neighbor_indexes[i]] == -1)
                insert_flags[j][neighbor_indexes[i]] = 1;
        }
    }

    printf("Finished creating random neighbors in %3.2f seconds\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);

    // Find K real and ALL reverse neighbors for each node
    // all_neighbors = (int **) malloc(data_size * sizeof(int *));
    // for(int i = 0; i < data_size; i++) { //k plus the max number of reverse neighbors
    //     all_neighbors[i] = (int *) malloc((data_size + maxNeighbors)* sizeof(int));
    // }


    start = clock();

    //for handling flags in search
    int **old = (int **) malloc(data_size * sizeof(int *));
    int **new = (int **) malloc(data_size * sizeof(int *));
    for(int i = 0; i < data_size; i++){
        old[i] = (int *) malloc((maxNeighbors)* sizeof(int));
        new[i] = (int *) malloc((maxNeighbors)* sizeof(int));
    }

    //These are not pre-allocated due to their variable size
    int **old_reverse = (int **) malloc(data_size * sizeof(int *));
    int **new_reverse = (int **) malloc(data_size * sizeof(int *));

    int ** joined_old_arrays = (int **) malloc(data_size * sizeof(int *));
    int ** joined_new_arrays = (int **) malloc(data_size * sizeof(int *));
    for(int i = 0; i < data_size; i++){
        joined_old_arrays[i] = (int *) malloc((data_size)* sizeof(int));
        joined_new_arrays[i] = (int *) malloc((data_size)* sizeof(int));
    }



    int sizes_t_flags[data_size]; 
    int sizes_f_flags[data_size];
    int sizes_t_flags_r[data_size];
    int sizes_f_flags_r[data_size];
    // main loop

    while(1){
        int update_counter = 0;
        printf("update_counter: %d\n", update_counter);
        // printAdjMatrix(myadjMatrix, data_size);
        for(int i = 0; i < data_size; i++){
            // get normal neighbors with false flag
            heap_to_array(neighbors[i],old[i],&sizes_f_flags[i], 0, insert_flags[i], sampling_rate);
            // get normal neighbors with true flag
            heap_to_array(neighbors[i],new[i],&sizes_t_flags[i], 1,  insert_flags[i], sampling_rate);
            for(int j = 0; j < sizes_t_flags[i]; j++){
                insert_flags[i][new[i][j]] = 0;
            }
            // get reverse neighbors with false flag
            old_reverse[i] = map_to_array(reverse_neighbors[i],&sizes_f_flags_r[i], 0,  insert_flags[i], sampling_rate);
            // get reverse neighbors with true flag
            new_reverse[i] = map_to_array(reverse_neighbors[i],&sizes_t_flags_r[i], 1,  insert_flags[i], sampling_rate);
            for(int j = 0; j < sizes_t_flags_r[i]; j++){
                insert_flags[i][new_reverse[i][j]] = 0;
            }

            // join normal neighbors with reverse neighbors
            joined_old_arrays[i] = join_arrays(old[i], sizes_f_flags[i], old_reverse[i], sizes_f_flags_r[i]);            
            joined_new_arrays[i] = join_arrays(new[i], sizes_t_flags[i], new_reverse[i], sizes_t_flags_r[i]);
            // printf("Size of new array is %d\n", sizes_t_flags[i] + sizes_t_flags_r[i]);
            // printf("Sizes are heap f: %d, map f %d, heap t %d, map t %d\n", sizes_f_flags[i], sizes_f_flags_r[i], sizes_t_flags[i], sizes_t_flags_r[i]);
        
        }

        //-----------------------------------------------------------------------------------//
        // for each node
        for(int i = 0; i < data_size; i++){
            // printf("Checking %d point\n", i);
            // for each neighbor
            int all_neighbors_count;
            
            int total_new_size = sizes_t_flags[i] + sizes_t_flags_r[i];
            int total_old_size = sizes_f_flags[i] + sizes_f_flags_r[i];
            
            //for j , k in joined_new_arrays[i], j < k--------------------------------------
            for(int j = 0; j < total_new_size; j++){
                int neighbor1 = joined_new_arrays[i][j];
                // for all neighbor pairs
                for(int k = j + 1; k < total_new_size; k++){
                    int neighbor2 = joined_new_arrays[i][k];
                                       
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

                    int old_neighbor1 = heap_find_max(neighbors[neighbor1]);
                    int old_neighbor2 = heap_find_max(neighbors[neighbor2]);
                    // if neighbor2 is not in neighbor1's neighbors
                    if(myadjMatrix[neighbor1][neighbor2] == 0){
                        if(heap_update(neighbors[neighbor1],neighbor2, weight) == true){
                            update_insert_flag(&insert_flags[neighbor1][neighbor2]);
                            // remove neighbor1 from old_neighbor1's neighbors, as we just replaced old_neighbor1 with neighbor2
                            map_remove(reverse_neighbors[old_neighbor1], neighbor1);
                            // add neighbor1 as reverse to its new neighbor
                            if(map_add(reverse_neighbors[neighbor2], neighbor1, weight))
                                update_insert_flag(&insert_flags[neighbor2][neighbor1]);
                            
                            //? update graph ???
                            removeEdge(myadjMatrix, neighbor1, old_neighbor1);
                            addEdge(myadjMatrix, neighbor1, neighbor2);
                            
                            update_counter++;
                        }
                    }

                    if (myadjMatrix[neighbor2][neighbor1] == 0){
                        if(heap_update(neighbors[neighbor2],neighbor1, weight) == true){
                            update_insert_flag(&insert_flags[neighbor2][neighbor1]);
                            // remove neighbor1 from old_neighbor1's neighbors, as we just replaced old_neighbor1 with neighbor2
                            map_remove(reverse_neighbors[old_neighbor2], neighbor2);
                            // add neighbor1 as reverse to its new neighbor
                            if(map_add(reverse_neighbors[neighbor1], neighbor2, weight))
                                update_insert_flag(&insert_flags[neighbor1][neighbor2]);
                            
                            //? update graph ???
                            removeEdge(myadjMatrix, neighbor2, old_neighbor2);
                            addEdge(myadjMatrix, neighbor2, neighbor1);
                            
                            update_counter++;
                        }
                    }
                }

                // for j in joined_new_arrays[i], k in joined_old_arrays[i]--------------------------------------
                for(int k = 1; k < total_old_size; k++){
                    int neighbor2 = joined_old_arrays[i][k];
                                       
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

                    int old_neighbor1 = heap_find_max(neighbors[neighbor1]);
                    int old_neighbor2 = heap_find_max(neighbors[neighbor2]);
                    // if neighbor2 is not in neighbor1's neighbors
                    if(myadjMatrix[neighbor1][neighbor2] == 0){
                        if(heap_update(neighbors[neighbor1],neighbor2, weight) == true){
                            update_insert_flag(&insert_flags[neighbor1][neighbor2]);
                            // remove neighbor1 from old_neighbor1's neighbors, as we just replaced old_neighbor1 with neighbor2
                            map_remove(reverse_neighbors[old_neighbor1], neighbor1);
                            // add neighbor1 as reverse to its new neighbor
                            if(map_add(reverse_neighbors[neighbor2], neighbor1, weight))
                                update_insert_flag(&insert_flags[neighbor2][neighbor1]);
                            
                            //? update graph ???
                            removeEdge(myadjMatrix, neighbor1, old_neighbor1);
                            addEdge(myadjMatrix, neighbor1, neighbor2);
                            
                            update_counter++;
                        }
                    }

                    if (myadjMatrix[neighbor2][neighbor1] == 0){
                        if(heap_update(neighbors[neighbor2],neighbor1, weight) == true){
                            update_insert_flag(&insert_flags[neighbor2][neighbor1]);
                            // remove neighbor1 from old_neighbor1's neighbors, as we just replaced old_neighbor1 with neighbor2
                            map_remove(reverse_neighbors[old_neighbor2], neighbor2);
                            // add neighbor1 as reverse to its new neighbor
                            if(map_add(reverse_neighbors[neighbor1], neighbor2, weight))
                                update_insert_flag(&insert_flags[neighbor1][neighbor2]);
                            
                            //? update graph ???
                            removeEdge(myadjMatrix, neighbor2, old_neighbor2);
                            addEdge(myadjMatrix, neighbor2, neighbor1);
                            
                            update_counter++;
                        }
                    }
                }
            }
        }
        // Early termination of algorithm if updates are less than the delta*KN threshold 
        printf("Update counter: %d of %d\n", update_counter, (int)(delta*maxNeighbors*data_size));
        if(update_counter  < delta*maxNeighbors*data_size){
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

    int **all_neighbors = (int **) malloc(data_size * sizeof(int *));
    int all_neighbors_count;
    int sizes[data_size];


    for(int i = 0; i < data_size; i++){
        all_neighbors[i] = (int *) malloc((data_size + maxNeighbors)* sizeof(int));
        getAllNeighbors(myadjMatrix, i, data_size, &all_neighbors_count, all_neighbors[i]);
    }
    // search here
    // search(myadjMatrix, dist_msr_ab, data, data_size, maxNeighbors, all_neighbors, sizes, flag);

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



void get_arguments(int argc, char** argv, int *maxNeighbors, float (**weight_fun)(void *, int, int, int), int *data_size, void** data, int* flag, double* delta, double* sampling_rate){
    if(argc < 5){
        //              0      1              2           3                4      5       6
        printf("Usage: ./main <maxNeighbors> <file_name> <weight_function> <flag> <delta> <sampling_rate>\n");
        exit(1);
    }
    *maxNeighbors = atoi(argv[1]);
    *flag = atoi(argv[4]);
    *delta = strtod(argv[5], NULL);
    *sampling_rate = strtod(argv[6], NULL);
    
    if(*flag == 0){  // competition data case
        *data = (Data)import_data(argv[2], data_size);
        if(*data == NULL){
            printf("Error in import_data\n");
            exit(1);
        }
    } else if(*flag == 1){   // ascii data case
        *data = (Data_tri)import_data_tri(argv[2], data_size);
    } else{ // false flag case
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag> <delta>\nso that flag = 0 || 1 \n");
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

    if(*delta > 1.0 || *delta <= 0){
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag> <delta>\nso that delta is in (0,1] \n");
        exit(1);
    }
    if(*sampling_rate > 1.0 || *sampling_rate <= 0){
        printf("Usage: ./main <maxNeighbors> <weight_function> <flag> <delta> <sampling_rate>\nso that sampling_rate is in (0,1] \n");
        exit(1);
    }

}


void update_insert_flag(int *insert_flag){
    if(*insert_flag == -1)
        *insert_flag = 1;
    else if(*insert_flag == 1){
        *insert_flag = 0;
    }
}