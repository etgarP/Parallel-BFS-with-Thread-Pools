#include "SyncGraph/graph.h"
#include <stdlib.h>
#include <stdio.h>

extern void bfs(Graph *graph, int **m);

int main(void) {
    int numVertices, numEdges;
    // Seed the random number generator
    srand(time(NULL));

    // Randomly determine number of vertices and edges
    numVertices = rand() % 10 + 5; // Random number of vertices between 5 and 14
    numEdges = rand() % (numVertices * (numVertices - 1) / 2); // Random number of edges (maximum possible)

    printf("Number of Vertices: %d, Number of Edges: %d\n", numVertices, numEdges);

    // Create graph
    Graph *graph = createGraph(numVertices);

    /* Create shortest paths matrix for starter graph */
    int **m1 = (int **) malloc(numVertices * sizeof(int *));
    for (int i = 0; i < numVertices; i++) {
        m1[i] = (int *) malloc(numVertices * sizeof(int));
    }

    // Add random edges to the graph
    printf("Adding %d random edges:\n", numEdges);
    for (int i = 0; i < numEdges; i++) {
        int source = rand() % numVertices;
        int destination = rand() % numVertices;
        addEdge(graph, source, destination);
        printf("(%d, %d) ", source, destination);

        m1[source][destination] = m1[destination][source] = 1;
    }
    printf("\n");
    /* Print BFS tree */
    printf("before Tree:\n");
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            printf("%d ", m1[i][j]);
        }
        putchar('\n');
    }

    printf("\n");

    /* Create shortest paths matrix */
    int **m = (int **) malloc(numVertices * sizeof(int *));
    for (int i = 0; i < numVertices; i++) {
        m[i] = (int *) malloc(numVertices * sizeof(int));
    }

    /* Perform BFS on the graph */
    bfs(graph, m);

    /* Print BFS tree */
    printf("BFS Tree:\n");
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            printf("%d ", m[i][j]);
        }
        putchar('\n');
    }

    /* Free memory */
    for (int i = 0; i < numVertices; i++) {
        free(m[i]);
    }
    free(m);

    return 0;
}
