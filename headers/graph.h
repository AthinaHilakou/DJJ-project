#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void addEdge(int** adjMatrix, int src, int dest);
void removeEdge(int** adjMatrix, int src, int dest);
int** createAdjMatrix(int numVertices, int maxNeighbors);
void getAllNeighbors(int** adjMatrix, int vertex, int numVertices, int* numNeighbors, int *neighbors);
void getNeighbors(int** adjMatrix, int vertex, int numVertices, int* numNeighbors, int *neighbors);
void getReverseNeighbors(int** adjMatrix, int vertex, int numVertices, int* numNeighbors, int *neighbors);
void printAdjMatrix(int** adjMatrix, int numVertices);
void freegraph(int** graph, int numVertices);
void printNeighbors(int *array, int size, int owner);
int **save_array(int **array, int size);