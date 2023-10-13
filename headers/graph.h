#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void addEdge(int** adjMatrix, int src, int dest);
int** createAdjMatrix(int numVertices, int maxNeighbors);
int* getNeighbors(int** adjMatrix, int vertex, int numVertices, int maxNeighbors);
void printAdjMatrix(int** adjMatrix, int numVertices, int maxNeighbors);
void freegraph(int** graph, int numVertices);