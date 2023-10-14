#include "../headers/graph.h"

int main() {
    int numVertices = 20;
    int maxNeighbors = 10;
    int** adjMatrix = createAdjMatrix(numVertices, maxNeighbors);

    printAdjMatrix(adjMatrix, numVertices);
    

    freegraph(adjMatrix, numVertices); // free graph memory
    

    return 0;

}