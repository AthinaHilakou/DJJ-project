#include "../headers/brute_force.h"
#include "../headers/min_heap.h"
int **brute_force(int k, float (*weight)(Data , int, int), Data data, int data_size){
	min_Heap *all_real_neighbors;
    all_real_neighbors = (min_Heap *)malloc(data_size*sizeof(min_Heap)); //real nughbors of each data point
	float weight_val;
	for(int i = 0; i < data_size; i++){ //initialize heaps 
		all_real_neighbors[i] = min_heap_create(NULL, 0, NULL);
	}

    for(int i = 0; i < data_size; i++){
		for(int j = 0; j < data_size; j++){
			if(i != j){ //if two points are neighbors insert them the heap 
				weight_val = weight(data,i,j);
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

	// ?print neighbors
	// for(int i = 0; i < data_size; i++){
	// 	printf("%2d: ", i);
	// 	for(int j = 0; j < k; j++){
	// 		printf("%d ", real_KNN[i][j]);
	// 	}
	// 	printf("\n");
	// }

    for(int i = 0; i < data_size; i++){
        min_heap_destroy(all_real_neighbors[i]);
    }
    free(all_real_neighbors);
    return real_KNN;

}

float recall(int **aprox_KNN, int k, float (*weight)(Data , int, int), Data data, int data_size){
    int ** real_KNN = brute_force(k, weight, data, data_size); //real neighbors of each data point
	int matches = 0;
	for(int i = 0; i < data_size; i++){ //for each data point
		for(int j = 0; j < k; j++){
			if( aprox_KNN[i][real_KNN[i][j]] == 1){
				matches++;
			}


			// if(aprox_KNN[f][i] == real_KNN[f][j]){
			// 	matches++; //count how many real neighbors are in the aprox KNN
			// 	continue;
			// }
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