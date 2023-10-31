#include "../headers/brute_force.h"
int **brute_force(int k, float (*weight)(Data *, int, int), Data *data, int data_size){
	Heap *all_real_neighbors;
    all_real_neighbors = (Heap *)malloc(data_size*sizeof(Heap)); //real nughbors of each data point
	float weight_val;
	for(int i = 0; i < data_size; i++){ //initialize heaps 
		all_real_neighbors[i] = heap_create(NULL, 0, NULL);
	}

    for(int i = 0; i < data_size; i++){
		for(int j = 0; j < data_size; j++){
			if(i != j){ //if two points are neighbors insert them the heap 
				weight_val = weight(data,i,j);
				heap_insert(all_real_neighbors[i], j, weight_val);
			}
		}
	}


	int **real_KNN = (int **)malloc(data_size * sizeof(int *)); //array of KNN real neighbors
	for(int i = 0; i < data_size; i++){
		real_KNN[i] = (int *)malloc(k * sizeof(int));
	}
	for(int i = 0; i < data_size; i++){
		for(int j = 0; j < k; j++){
			real_KNN[i][j] = index_from_heap(all_real_neighbors[i], j);
		}
	}

    for(int i = 0; i < data_size; i++){
        heap_destroy(all_real_neighbors[i]);
    }
    free(all_real_neighbors);
    return real_KNN;

}

float recall(int **aprox_KNN, int k, float (*weight)(Data *, int, int), Data *data, int data_size){
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
	printf("recall: %f\n", myrecall);
	return myrecall; //accuracy measure for KNN descent algorithm
}