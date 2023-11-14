#include "../headers/brute_force.h"
#include "../headers/min_heap.h"
#include "../headers/data.h"
int **brute_force(int k, float (*weight)(void* , int, int, int), void* data, int data_size, int flag){
	min_Heap *all_real_neighbors;
    all_real_neighbors = (min_Heap *)malloc(data_size*sizeof(min_Heap)); //real nughbors of each data point
	float weight_val;
	for(int i = 0; i < data_size; i++){ //initialize heaps 
		all_real_neighbors[i] = min_heap_create(NULL, 0, NULL);
	}

    for(int i = 0; i < data_size; i++){
		for(int j = 0; j < data_size; j++){
			if(i != j){ //if two points are neighbors insert them the heap 
				weight_val = weight(data, i, j, flag);
				min_heap_insert(all_real_neighbors[i], j, weight_val);
			}
		}
	}


	int **real_KNN = (int **)malloc(data_size * sizeof(int *)); //array of KNN real neighbors
	for(int i = 0; i < data_size; i++){
		real_KNN[i] = (int *)malloc(k * sizeof(int));
	}
	for(int i = 0; i < data_size; i++){
		for(int j = 0; j < k; j++){
			real_KNN[i][j] = index_from_min_heap(all_real_neighbors[i], j);
		}
	}


    for(int i = 0; i < data_size; i++){
        min_heap_destroy(all_real_neighbors[i]);
    }
    free(all_real_neighbors);
    return real_KNN;

}

float recall(int **aprox_KNN, int k, float (*weight)(void*, int, int, int), void* data, int data_size, int flag){
    int ** real_KNN = brute_force(k, weight, data, data_size, flag); //real neighbors of each data point
	int matches = 0;
	for(int i = 0; i < data_size; i++){ //for each data point
		for(int j = 0; j < k; j++){
			if( aprox_KNN[i][real_KNN[i][j]] == 1){
				matches++;
			}
		}
	}

	printf("matches: %d of %d\n", matches, data_size * k);

	for(int i = 0; i < data_size; i++){ //free real_KNN
		free(real_KNN[i]);
	}
	free(real_KNN);
	float myrecall = (float)matches / (float)(data_size * k);
	return myrecall; //accuracy measure for KNN descent algorithm
}

// returns a random float between 0 and 1
float random_float() {
    return (float)rand() / (float)RAND_MAX;
}

// makes a random data point and searches for its k nearest neighbors on graph
void search(int **graph, float weight_fun(void*, void*, int), void* my_data, int data_size, int k, int**all_neighbors, int *sizes, int flag){
	// create random search point
	srand(time(NULL));
	void *search_point;
	if(flag == 0){
		Data S_Point = (Data) malloc(sizeof(data));
		search_point = S_Point;
		for(int i = 0; i < DATA_LENTH; i++){
			S_Point->data_array[i] = random_float();
		}
	} else{
		Data_tri S_Point = (Data_tri) malloc(sizeof(data_tri));
		search_point = S_Point;
		for(int i = 0; i < DATA_LENTH_TRI; i++){
			S_Point->data_array[i] = rand() % 1000;
		}
	}

	// k nearest neighbors indexes
	int* neighbor_indexes = (int*)malloc(k*sizeof(int));
	// k nearest neighbors weights
	float *weights = (float *)malloc(data_size*sizeof(float));
	// array to check if a neighbor has been selected
	int *neighbor_array = (int *)calloc(data_size, sizeof(int));

    for(int i = 0; i < k;){
        int num = rand() % data_size;		// find a random neighbor
        if (!neighbor_array[num]) {
            neighbor_indexes[i] = num;
            neighbor_array[num] = 1;
            i++;
        }
    }
	// initialize heap

	if(flag == 0){
		Data data_array = (Data)my_data;
		for(int i = 0; i < k; i++){
			weights[i] = weight_fun((void *) &data_array[neighbor_indexes[i]], search_point, flag);
		}
	} else{
		Data_tri data_array = (Data_tri)my_data;
		for(int i = 0; i < k; i++){
			weights[i] = weight_fun((void *) &data_array[neighbor_indexes[i]], search_point, flag);
		}
	}

	Heap neighbors = heap_create(neighbor_indexes, k, weights);
	float *weights_array = (float *)malloc(data_size*sizeof(float));
	for(int i = 0; i < data_size; i++){
		weights_array[i] = -1;
	}

    // main loop
    while(1){
        int update_counter = 0;
            // for each neighbor   
		//update neighbor_indexes based on neighbors array
		for(int i = 0; i < k; i++){
			neighbor_indexes[i] = index_from_heap(neighbors, i);
		}        
		for(int i = 0; i < k; i++){
			//find num of neighbors of neighbor
			int neighbor_index = neighbor_indexes[i];
			int neighbor_neighbor_size = sizes[neighbor_index];
			// for each neighbor of neighbor
			for(int j = 0; j < neighbor_neighbor_size; j++){
				int neighbor_neighbor_index = all_neighbors[neighbor_index][j];

				float weight;
				if(weights_array[neighbor_neighbor_index] - 1 != 0){
					if(flag == 0){
						Data data_array = (Data)my_data;
						weight = weight_fun((void *) &data_array[neighbor_neighbor_index], search_point, flag);
					} else{
						Data_tri data_array = (Data_tri)my_data;
						weight = weight_fun((void *) &data_array[neighbor_neighbor_index], search_point, flag);
					}
				} else{
					weight = weights_array[neighbor_neighbor_index];
				}
				//get furthest neighbor, from k nearest
				int old_neighbor = index_from_heap(neighbors,0);
				if(weight){ //ensure that neighbor's neighbor is not i itself AND that the edge exists
					if(neighbor_array[neighbor_neighbor_index] == 1){
						continue;
					}
					if(heap_update(neighbors, neighbor_neighbor_index, weight) == true){
						update_counter++;
						neighbor_array[old_neighbor] = 0;
						neighbor_array[neighbor_neighbor_index] = 1;  
					}
				}
			}
		}
        if(update_counter == 0){
            break;
        }
    }

	for(int i = 0; i < k; i++){
		neighbor_indexes[i] = index_from_heap(neighbors, i);
	}

	//brute search section
	min_Heap all_real_neighbors;
	all_real_neighbors = min_heap_create(NULL, 0, NULL);
	float *all_weights = malloc(sizeof(float)*data_size);
	int *real_neighbor_indexes = malloc(sizeof(int)*k);
	if(flag == 0){
		Data data_array = (Data)my_data;
		for(int i = 0; i < data_size; i++){
			all_weights[i] = weight_fun((void *) &data_array[i], search_point, flag);
		}
	} else{
		Data_tri data_array = (Data_tri)my_data;
		for(int i = 0; i < data_size; i++){
			all_weights[i] = weight_fun((void *) &data_array[i], search_point, flag);
		}
	}

	for(int i = 0; i < data_size; i++){
		min_heap_insert(all_real_neighbors, i, all_weights[i]);
	}
	for(int i = 0; i < k; i++){
		real_neighbor_indexes[i] = min_heap_pop(all_real_neighbors);
	}
	int matches = 0;
	for(int i = 0; i < k; i++){
		if(neighbor_array[real_neighbor_indexes[i]] == 1){
			matches++;
		}
	}
	printf("recall of search is %1.3f\n", ((float)matches/(float)k));

	// free memory
	free(all_weights);
	free(real_neighbor_indexes);
	free(neighbor_indexes);	
	free(weights);	
	free(neighbor_array);
	free(weights_array);
	heap_destroy(neighbors);
	min_heap_destroy(all_real_neighbors);
}