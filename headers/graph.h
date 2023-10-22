#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void addEdge(int** adjMatrix, int src, int dest);
int** createAdjMatrix(int numVertices, int maxNeighbors);
int* getAllNeighbors(int** adjMatrix, int vertex, int numVertices, int* numNeighbors);
int* getNeighbors(int** adjMatrix, int vertex, int numVertices, int* numNeighbors);
int* getReverseNeighbors(int** adjMatrix, int vertex, int numVertices, int* numNeighbors);
void printAdjMatrix(int** adjMatrix, int numVertices);
void freegraph(int** graph, int numVertices);