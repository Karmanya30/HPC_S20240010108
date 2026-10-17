// Q5: BFS and DFS with OpenMP on a small graph (adjacency matrix)
// Compile: gcc -fopenmp q5_bfs_dfs.c -o q5
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define V 2000                       // number of vertices

int adj[V][V];                       // adj[a][b] = 1 if edge a-b
int visited[V];

// ---------------- BFS (parallel, level by level) ----------------
void bfs(int start) {
    int frontier[V], next[V];
    int frontier_size = 1, next_size;
    int level = 0;

    for (int i = 0; i < V; i++) visited[i] = 0;
    frontier[0] = start;
    visited[start] = 1;

    while (frontier_size > 0) {
        next_size = 0;

        // all vertices of the current level are processed at the same time
        #pragma omp parallel for
        for (int i = 0; i < frontier_size; i++) {
            int u = frontier[i];
            for (int v = 0; v < V; v++) {
                if (adj[u][v] && !visited[v]) {
                    #pragma omp critical      // one thread at a time -> no race
                    {
                        if (!visited[v]) {    // check again inside the critical part
                            visited[v] = 1;
                            next[next_size++] = v;
                        }
                    }
                }
            }
        }

        // next level becomes the current level
        for (int i = 0; i < next_size; i++) frontier[i] = next[i];
        frontier_size = next_size;
        level++;
    }
    printf("BFS finished, %d levels\n", level);
}

// ---------------- DFS (parallel with tasks) ----------------
void dfs(int u, int depth) {
    for (int v = 0; v < V; v++) {
        if (adj[u][v]) {
            int go = 0;
            #pragma omp critical              // claim vertex v safely
            {
                if (!visited[v]) { visited[v] = 1; go = 1; }
            }
            if (go) {
                if (depth < 4) {              // only create tasks near the top
                    #pragma omp task
                    dfs(v, depth + 1);
                } else {
                    dfs(v, depth + 1);        // deeper: normal recursive call
                }
            }
        }
    }
}

int count_visited() {
    int c = 0;
    for (int i = 0; i < V; i++) c += visited[i];
    return c;
}

int main() {
    // build a random graph (each vertex gets ~5 random neighbours)
    srand(1);
    for (int a = 0; a < V; a++)
        for (int k = 0; k < 5; k++) {
            int b = rand() % V;
            adj[a][b] = adj[b][a] = 1;
        }

    bfs(0);
    printf("BFS visited %d vertices\n", count_visited());

    for (int i = 0; i < V; i++) visited[i] = 0;
    visited[0] = 1;
    #pragma omp parallel
    {
        #pragma omp single
        {
            dfs(0, 0);
            #pragma omp taskwait
        }
    }
    printf("DFS visited %d vertices\n", count_visited());
    return 0;
}
