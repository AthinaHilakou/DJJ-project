#include "../headers/graph.h"
#include "../headers/acutest.h"


void test_addEdge(void) {
    int numVertices = 5;
    int** adjMatrix = createAdjMatrix(numVertices, 2);

    addEdge(adjMatrix, 0, 1);
    addEdge(adjMatrix, 1, 2);
    addEdge(adjMatrix, 2, 3);
    addEdge(adjMatrix, 3, 4);

    TEST_CHECK(adjMatrix[0][1] == 1);
    TEST_CHECK(adjMatrix[1][2] == 1);
    TEST_CHECK(adjMatrix[2][3] == 1);
    TEST_CHECK(adjMatrix[3][4] == 1);

    freegraph(adjMatrix, numVertices);
}

void test_removeEdge(void) {
    int numVertices = 5;
    int** adjMatrix = createAdjMatrix(numVertices, 2);

    addEdge(adjMatrix, 0, 1);
    addEdge(adjMatrix, 1, 2);
    addEdge(adjMatrix, 2, 3);
    addEdge(adjMatrix, 3, 4);

    removeEdge(adjMatrix, 1, 2);
    removeEdge(adjMatrix, 3, 4);

    TEST_CHECK(adjMatrix[0][1] == 1);
    TEST_CHECK(adjMatrix[1][2] == 0);
    TEST_CHECK(adjMatrix[2][3] == 1);
    TEST_CHECK(adjMatrix[3][4] == 0);

    freegraph(adjMatrix, numVertices);
}

void test_getNeighbors(void) {
    int numVertices = 5;
    int** adjMatrix = createAdjMatrix(numVertices, 2);

    int numNeighbors;
    int neighbors[5];

    getNeighbors(adjMatrix, 0, numVertices, &numNeighbors, neighbors);
    TEST_CHECK(numNeighbors == 2);
    TEST_CHECK(neighbors[0] != neighbors[1]);
    TEST_CHECK(neighbors[0] > 0);
    TEST_CHECK(neighbors[0] <= 4);
    TEST_CHECK(neighbors[1] > 0);
    TEST_CHECK(neighbors[1] <= 4);

    getNeighbors(adjMatrix, 2, numVertices, &numNeighbors, neighbors);
    TEST_CHECK(numNeighbors == 2);
    TEST_CHECK(neighbors[0] != 2);
    TEST_CHECK(neighbors[1] != 2);
    TEST_CHECK(neighbors[0] != neighbors[1]);
    TEST_CHECK(neighbors[0] >= 0);
    TEST_CHECK(neighbors[0] <= 4);
    TEST_CHECK(neighbors[1] >= 0);
    TEST_CHECK(neighbors[1] <= 4);



    freegraph(adjMatrix, numVertices);
}

void test_getReverseNeighbors(void) {
    int numVertices = 5;
    int maxNeighbors = 3;
    int** adjMatrix = createAdjMatrix(numVertices, maxNeighbors);

    int numReverseNeighbors;
    int ReverseNeighbors[5];

    int numNeighbors;
    int neighbors[5];

    getReverseNeighbors(adjMatrix, 0, numVertices, &numReverseNeighbors, ReverseNeighbors);
    TEST_CHECK(ReverseNeighbors > 0);
    getNeighbors(adjMatrix, ReverseNeighbors[0], numVertices, &numNeighbors, neighbors);

    TEST_CHECK(numNeighbors == 3);
    // assert that the reverse neighbor is in the list of the neighbors
    TEST_CHECK(neighbors[0] == 0 || neighbors[1] == 0 || neighbors[2] == 0);

    freegraph(adjMatrix, numVertices);
}

void test_getAllNeighbors(void) {
    int numVertices = 5;
    int maxNeighbors = 2;
    int** adjMatrix = createAdjMatrix(numVertices, maxNeighbors);

    int numNeighbors;
    int neighbors[5];

    int numReverseNeighbors;
    int ReverseNeighbors[5];

    int all_neighbors[5];
    int all_neighbors_count;
    
    getNeighbors(adjMatrix, 0, numVertices, &numNeighbors, neighbors);
    getReverseNeighbors(adjMatrix, 0, numVertices, &numReverseNeighbors, ReverseNeighbors);
    getAllNeighbors(adjMatrix, 0, numVertices, &all_neighbors_count, all_neighbors);

    TEST_CHECK(numNeighbors == maxNeighbors);
    TEST_CHECK(all_neighbors_count <= numNeighbors + numReverseNeighbors);
    TEST_CHECK(all_neighbors_count <= numVertices);

    freegraph(adjMatrix, numVertices);
}

void test_printAdjMatrix(void) {
    int numVertices = 5;
    int** adjMatrix = createAdjMatrix(numVertices, 2);

    // Redirect stdout to a file
    FILE* file = freopen("test_output.txt", "w", stdout);
    TEST_CHECK(file != NULL);

    printAdjMatrix(adjMatrix, numVertices);

    // Restore stdout
    fclose(file);

    // Read the contents of the file
    file = fopen("test_output.txt", "r");

    TEST_CHECK(file != NULL);

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);


    // Check if the output matches the expected output
    TEST_CHECK(strcmp(buffer, "Adjacency Matrix:\n") == 0);

    fclose(file);
    freegraph(adjMatrix, numVertices);
}

TEST_LIST = {
    {"test_addEdge", test_addEdge},
    {"test_removeEdge", test_removeEdge},
    {"test_getNeighbors", test_getNeighbors},
    {"test_getReverseNeighbors", test_getReverseNeighbors},
    {"test_getAllNeighbors", test_getAllNeighbors},
    // {"test_printAdjMatrix", test_printAdjMatrix},
    {NULL, NULL}
};
