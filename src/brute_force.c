#include "../headers/brute_force.h"
int **brute_force(int k, float (*weight)(Data *, int, int), Data *data, int data_size){
	Heap *all_real_neighbors;
    all_real_neighbors = (Heap *)malloc(data_size*sizeof(Heap)); //real nughbors of each data point
	int weight_val;
	for(int i = 0; i < data_size; i++){ //initialize heaps 
		all_real_neighbors[i] = heap_create(NULL, 0, NULL);
	}

    for(int i = 0; i < data_size; i++){
    	printf("i: %d, ", i); 
		for(int j = 0; j < data_size; j++){

			if((weight_val = weight(data,i,j)) && (i != j)){ //if two points are neighbors insert them the heap 
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
	for(int f = 0; f < data_size; f++){ //for each data point
		for(int i = 0; i < k; i++){
			for(int j = 0; j < k; j++){
				if(aprox_KNN[f][i] == real_KNN[f][j]){
					matches++; //count how many real neighbors are in the aprox KNN
					continue;
				}
			}
		}
	}

	for(int i = 0; i < data_size; i++){ //free real_KNN
		free(real_KNN[i]);
	}
	free(real_KNN);
	float recall = (float)(matches/(data_size*k));
	return recall; //accuracy measure for KNN descent algorithm
}