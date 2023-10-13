#include "../headers/graph.h"

int main() {
    int numVertices = 5;
    int maxNeighbors = 3;
    int** adjMatrix = createAdjMatrix(numVertices, maxNeighbors);

    printAdjMatrix(adjMatrix, numVertices, maxNeighbors);
    

    freegraph(adjMatrix, numVertices); // free graph memory
    

    return 0;

}