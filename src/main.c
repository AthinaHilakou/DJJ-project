#include "../headers/graph.h"
#include "../headers/data_tri.h"
#include "../headers/heap.h"
#include "../headers/data.h"
#include <stdlib.h>
#include <sys/mman.h>

int main() {
    int numVertices = 20;
    int maxNeighbors = 10;

    Data_tri *data = (Data_tri *) import_data("data.txt", sizeof(Data_tri));
    
    Heap *neighbors;
    neighbors = (Heap *) malloc(numVertices * sizeof(Heap));
    srand(time(NULL)); // seed random number generator
    int* adjMatrix = (int*) malloc(numVertices * sizeof(int));
    for(int j = 0; j < numVertices; j++) {
        for (int i = 0; i < numVertices; i++) {
            int randNum; // random number between 0 and numVertices - 1
            do{ // generate a new neighbor
                randNum = rand() % numVertices;
            }while (adjMatrix[randNum] == 1 || randNum == i);    // dont allow self neighbors or repeat neighbors
            adjMatrix[i] = randNum;
        }
        heap_create(adjMatrix,(void *) &data[j], dist_msr, maxNeighbors, maxNeighbors, sizeof(Data_tri));
    }
    
    

    return 0;

}