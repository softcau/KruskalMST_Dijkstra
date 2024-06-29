#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int src;
    int dest;
    int weight;
} Edge;

typedef struct {
    int V, E;
    Edge* edge;
} Graph;

// 그래프 생성
Graph* createGraph(int V, int E) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge*)malloc(graph->E * sizeof(Edge));
    return graph;
}

// 그래프 출력
void printGraph(Graph* graph) {
    printf("[ Input Graph Information ]\n");
    printf("src - dest : weight\n");
    for (int i = 0; i < graph->E; i++) {
        printf(" %d  -  %d   :  %d\n", graph->edge[i].src, graph->edge[i].dest, graph->edge[i].weight);
    }
    printf("\n");
}

// 비교 함수
int compare(const void* x, const void* y) {
    Edge* edge_x = (Edge*)x;
    Edge* edge_y = (Edge*)y;
    return edge_x->weight > edge_y->weight;
}

// Find
int find(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return find(parent, parent[i]);
}

// Union
void Union(int parent[], int rank[], int root_of_src, int root_of_dest) {
    if (rank[root_of_src] < rank[root_of_dest]) {
        parent[root_of_src] = root_of_dest;
    }
    else if (rank[root_of_src] > rank[root_of_dest]) {
        parent[root_of_dest] = root_of_src;
    }
    else {
        parent[root_of_dest] = root_of_src;
        rank[root_of_src]++;
    }
}

void Kruskal(Graph* graph) {

    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), compare);

    Edge* edge_in_MST = (Edge*)malloc(graph->V * sizeof(Edge));
    int e = 0;
    int k = 0;

    // 부모, rank 초기화 (0번 위치는 비움)
    int* parent = (int*)malloc((graph->V + 1) * sizeof(int));
    int* rank = (int*)malloc((graph->V + 1) * sizeof(int));

    for (int v = 1; v <= graph->V; v++) {
        parent[v] = v;
        rank[v] = 0;
    }

    while (e < graph->V - 1 && k < graph->E) {
        Edge selected_edge = graph->edge[k];
        k++;

        int root_of_src = find(parent, selected_edge.src);
        int root_of_dest = find(parent, selected_edge.dest);

        if (root_of_src != root_of_dest) {
            edge_in_MST[e] = selected_edge;
            e++;
            Union(parent, rank, root_of_src, root_of_dest);
        }
    }
    printf("[ MST Result ]\n");
    printf("src - dest : weight\n");
    for (k = 0; k < e; k++) {
        printf(" %d  -  %d   :  %d\n", edge_in_MST[k].src, edge_in_MST[k].dest, edge_in_MST[k].weight);
    }
    printf("\n");
    free(parent);
    free(rank);
    free(edge_in_MST);
}

// 최소 거리를 가지는 정점을 반환하는 함수
int minDistance(int dist[], int visited[], int V) {
    int min = INT_MAX; // 현재까지 발견된 최소 거리(초기값은 INT_MAX)
    int min_index = 0; // 최소 거리를 가진 정점의 인덱스 저장할 변수
    for (int v = 1; v <= V; v++)
        if (visited[v] == 0 && dist[v] <= min){
            min = dist[v];
            min_index = v;
        }
    return min_index;
}

void printPath(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%d", j);
        return;
    }
    printPath(parent, parent[j]);
    printf(" -> %d", j);
}

void Dijkstra(Graph* graph, int src) {
    int* dist = (int*)malloc((graph->V + 1) * sizeof(int));
    int* visited = (int*)malloc((graph->V + 1) * sizeof(int)); 
    int* parent = (int*)malloc((graph->V + 1) * sizeof(int)); 

    for (int i = 1; i <= graph->V; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[src] = 0;

    for (int count = 1; count <= graph->V - 1; count++) {
        int u = minDistance(dist, visited, graph->V);
        visited[u] = 1;

        for (int i = 0; i < graph->E; i++) {
            if (graph->edge[i].src == u || graph->edge[i].dest == u) {
                int v = (graph->edge[i].src == u) ? graph->edge[i].dest : graph->edge[i].src;
                if (!visited[v] && dist[u] + graph->edge[i].weight < dist[v]) {
                    dist[v] = dist[u] + graph->edge[i].weight;
                    parent[v] = u;
                }
            }
        }
    }

    printf("[ Dijkstra Result ]\n");
    printf("Vertex# \t Distance from vertex 1 \tShortest Path\n");
    for (int i = 1; i <= graph->V; i++) {
        printf("%d \t\t %d \t\t\t\t", i, dist[i]);
        printPath(parent, i);
        printf("\n");
    }

    free(dist);
    free(visited);
    free(parent);
}

int main() {
    int V = 16;
    int E = 24;
    Graph* graph = createGraph(V, E);

    // 문제에서 제시된 edge 정보 추가
    {
        graph->edge[0].src = 1;
        graph->edge[0].dest = 2;
        graph->edge[0].weight = 3;

        graph->edge[1].src = 2;
        graph->edge[1].dest = 3;
        graph->edge[1].weight = 1;

        graph->edge[2].src = 3;
        graph->edge[2].dest = 4;
        graph->edge[2].weight = 4;

        graph->edge[3].src = 5;
        graph->edge[3].dest = 6;
        graph->edge[3].weight = 1;

        graph->edge[4].src = 6;
        graph->edge[4].dest = 7;
        graph->edge[4].weight = 5;

        graph->edge[5].src = 7;
        graph->edge[5].dest = 8;
        graph->edge[5].weight = 8;

        graph->edge[6].src = 9;
        graph->edge[6].dest = 10;
        graph->edge[6].weight = 2;

        graph->edge[7].src = 10;
        graph->edge[7].dest = 11;
        graph->edge[7].weight = 4;

        graph->edge[8].src = 11;
        graph->edge[8].dest = 12;
        graph->edge[8].weight = 6;

        graph->edge[9].src = 13;
        graph->edge[9].dest = 14;
        graph->edge[9].weight = 3;

        graph->edge[10].src = 14;
        graph->edge[10].dest = 15;
        graph->edge[10].weight = 5;

        graph->edge[11].src = 15;
        graph->edge[11].dest = 16;
        graph->edge[11].weight = 7;

        graph->edge[12].src = 1;
        graph->edge[12].dest = 5;
        graph->edge[12].weight = 2;

        graph->edge[13].src = 5;
        graph->edge[13].dest = 9;
        graph->edge[13].weight = 3;

        graph->edge[14].src = 9;
        graph->edge[14].dest = 13;
        graph->edge[14].weight = 2;

        graph->edge[15].src = 2;
        graph->edge[15].dest = 6;
        graph->edge[15].weight = 5;

        graph->edge[16].src = 6;
        graph->edge[16].dest = 10;
        graph->edge[16].weight = 4;

        graph->edge[17].src = 10;
        graph->edge[17].dest = 14;
        graph->edge[17].weight = 6;

        graph->edge[18].src = 3;
        graph->edge[18].dest = 7;
        graph->edge[18].weight = 1;

        graph->edge[19].src = 7;
        graph->edge[19].dest = 11;
        graph->edge[19].weight = 2;

        graph->edge[20].src = 11;
        graph->edge[20].dest = 15;
        graph->edge[20].weight = 4;

        graph->edge[21].src = 4;
        graph->edge[21].dest = 8;
        graph->edge[21].weight = 6;

        graph->edge[22].src = 8;
        graph->edge[22].dest = 12;
        graph->edge[22].weight = 6;

        graph->edge[23].src = 12;
        graph->edge[23].dest = 16;
        graph->edge[23].weight = 4;

    }

    printGraph(graph);

    Kruskal(graph);
    Dijkstra(graph, 1);

    free(graph->edge);
    free(graph);

    return 0;
}
