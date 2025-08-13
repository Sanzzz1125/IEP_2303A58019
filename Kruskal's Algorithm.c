#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Edge {
    int src, dest, weight;
};

struct Subset {
    int parent;
    int rank;
};

int find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

void Union(struct Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int compareEdges(const void *a, const void *b) {
    return ((struct Edge *)a)->weight - ((struct Edge *)b)->weight;
}

void KruskalMST(struct Edge edges[], int V, int E, char vertices[][10]) {
    qsort(edges, E, sizeof(edges[0]), compareEdges);

	int v;
    struct Subset *subsets = (struct Subset *)malloc(V * sizeof(struct Subset));
    for (v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    struct Edge result[V]; // Store MST edges
    int e = 0; // index for result[]
    int i = 0; // index for edges[]

    while (e < V - 1 && i < E) {
        struct Edge next_edge = edges[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        if (x != y) {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
    }

    printf("Selected Edges in the MST:\n");
    int minCost = 0;
    for (i = 0; i < e; ++i) {
        printf("%s - %s : %d\n",
               vertices[result[i].src],
               vertices[result[i].dest],
               result[i].weight);
        minCost += result[i].weight;
    }
    printf("\nTotal Minimum Cost = %d\n", minCost);

    free(subsets);
}

int main() {
    int V = 5; // Number of buildings
    int E = 8; // Number of possible connections

    char vertices[5][10] = {"L", "H", "C", "B", "A"}; 
    // L=Library, H=Hostel, C=Classroom, B=Lab, A=Admin

    struct Edge edges[] = {
        {0, 1, 4}, // L-H
        {0, 2, 3}, // L-C
        {0, 3, 5}, // L-B
        {1, 2, 6}, // H-C
        {1, 4, 5}, // H-A
        {2, 3, 4}, // C-B
        {2, 4, 7}, // C-A
        {3, 4, 2}  // B-A
    };

    KruskalMST(edges, V, E, vertices);

    return 0;
}
