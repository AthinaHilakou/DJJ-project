#include "../headers/graph.h"

// adds an edge from src to dest in the adjacency matrix
// (does not add the corresponding edge from dest to src)
void addEdge(int** adjMatrix, int src, int dest) {
    adjMatrix[src][dest] = 1;
    // adjMatrix[dest][src] = 1; // commented out to make the matrix non-symmetric
}

// creates a random adjacency matrix with numVertices vertices and maxNeighbors neighbors per vertex
// each edge has a weight of 1
int** createAdjMatrix(int numVertices, int maxNeighbors) {
    if(maxNeighbors > numVertices) {
        printf("Error: maxNeighbors cannot be greater than numVertices\n");
        return NULL;
    }
    srand(time(NULL)); // seed random number generator
    int** adjMatrix = (int**) malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        adjMatrix[i] = (int*) calloc(numVertices, sizeof(int));
        for (int j = 0; j < maxNeighbors; j++) {
            int randNum = rand() % numVertices; // generate random number between 0 and numVertices - 1
            while (adjMatrix[i][randNum] == 1) { // if there is already an edge from i to randNum, generate a new random number
                randNum = rand() % numVertices;
            }
            adjMatrix[i][randNum] = 1; // set edge from i to randNum
        }
    }
    return adjMatrix;
}


// returns an array of the neighbors for the given vertex
int* getNeighbors(int** adjMatrix, int vertex, int numVertices, int maxNeighbors) {
    int* neighbors = (int*) malloc(maxNeighbors * sizeof(int));
    int count = 0;
    for (int i = 0; i < maxNeighbors; i++) {
        if (adjMatrix[vertex][i] == 1) {
            neighbors[count] = i;
            count++;
        }
    }
    return neighbors;
}

void printAdjMatrix(int** adjMatrix, int numVertices, int maxNeighbors) {
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < maxNeighbors; j++) {
            printf("%d ", adjMatrix[i][j]);
        }
        printf("\n");
    }
}

void freegraph(int** graph, int numVertices) {
    for (int i = 0; i < numVertices; i++) {
        free(graph[i]); // free adjacency matrix memory
    }
    free(graph); // free graph memory
}