#include <stdio.h>
#include <limits.h>

#define MAX_NODES 5
#define INF INT_MAX

// Function to perform Distance Vector Routing
void distanceVectorRouting(int costMatrix[][MAX_NODES], int nodes) {
    int distance[MAX_NODES];
    int nextHop[MAX_NODES];

    // Initialize distance and nextHop arrays
    for (int i = 0; i < nodes; i++) {
        distance[i] = INF;
        nextHop[i] = -1;
    }

    // Set distance to self as 0
    for (int i = 0; i < nodes; i++) {
        distance[i] = 0;
    }

    // Perform distance vector updates
    for (int k = 0; k < nodes; k++) {
        for (int i = 0; i < nodes; i++) {
            for (int j = 0; j < nodes; j++) {
                if (costMatrix[i][j] != INF && distance[i] + costMatrix[i][j] < distance[j]) {
                    distance[j] = distance[i] + costMatrix[i][j];
                    nextHop[j] = i;
                }
            }
        }
    }

    // Print the routing table
    printf("Routing Table:\n");
    printf("Destination\tNext Hop\tDistance\n");
    for (int i = 0; i < nodes; i++) {
        printf("%d\t\t%d\t\t%d\n", i, nextHop[i], distance[i]);
    }
}

int main() {
    int costMatrix[MAX_NODES][MAX_NODES] = {
        {0, 2, INF, 1, INF},
        {2, 0, 1, INF, 3},
        {INF, 1, 0, 4, INF},
        {1, INF, 4, 0, 2},
        {INF, 3, INF, 2, 0}
    };

    int nodes = MAX_NODES;

    // Perform Distance Vector Routing
    distanceVectorRouting(costMatrix, nodes);

    return 0;
}
