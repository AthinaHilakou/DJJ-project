#include "../headers/graph.h"

// adds an edge from src to dest in the adjacency matrix
void addEdge(int** adjMatrix, int src, int dest) {
    adjMatrix[src][dest] = 1;
    // adjMatrix[dest][src] = 1; // uncommented to make the matrix symmetric
}

// removes an edge from src to dest in the adjacency matrix
void removeEdge(int** adjMatrix, int src, int dest) {
    adjMatrix[src][dest] = 0;
    // adjMatrix[dest][src] = 0; // uncommented to make the matrix symmetric
}

// creates a random adjacency matrix with numVertices vertices and maxNeighbors neighbors per vertex
int** createAdjMatrix(int numVertices, int maxNeighbors) {
    if(maxNeighbors >= numVertices) {
        printf("Error: maxNeighbors cannot be greater than numVertices\n");
        return NULL;
    }
    srand(time(NULL)); // seed random number generator
    int** adjMatrix = (int**) malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        adjMatrix[i] = (int*) calloc(numVertices, sizeof(int));
        for (int j = 0; j < maxNeighbors; j++) {
            int randNum; // generate random number between 0 and numVertices - 1
            do{ // if there is already an edge from i to randNum, generate a new random number
                randNum = rand() % numVertices;
            }while (adjMatrix[i][randNum] == 1 || randNum == i);    // dont allow self neighbors
            // int weight = rand() % 1000 + 1; // generate random weight between 1 and 1000
            // TODO: save the weight in the adjacency matrix
            adjMatrix[i][randNum] = 1; // set edge from i to randNum
            // addEdge(adjMatrix, i, randNum);
        }
    }
    return adjMatrix;
}

// returns an array of the neighbors for the given vertex
int* getNeighbors(int** adjMatrix, int vertex, int numVertices) {
    int* neighbors = (int*) malloc(numVertices * sizeof(int));
    int count = 0;
    for (int i = 0; i < numVertices; i++) {
        if (adjMatrix[vertex][i] == 1) {
            neighbors[count] = i;
            count++;
        }
    }
    return neighbors;
}

int* getReverseNeighbors(int** adjMatrix, int vertex, int numVertices) {
    int* neighbors = (int*) malloc(numVertices * sizeof(int));
    int count = 0;
    for (int i = 0; i < numVertices; i++) {
        if (adjMatrix[i][vertex] == 1) {
            neighbors[count] = i;
            count++;
        }
    }
    return neighbors;
} 

void printAdjMatrix(int** adjMatrix, int numVertices) {
    printf("Adjacency Matrix:\n");
    for (int i = 0; i < numVertices; i++) {
        printf("%3d: ", i);
        for (int j = 0; j < numVertices; j++) {
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