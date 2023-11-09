#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 20
#define INF INT_MAX

typedef struct Node
{
    int vertex;
    int weight;
    struct Node* next;
} Node;

typedef struct GraphType
{
    int n;
    Node* adj_list[MAX_VERTICES];
} GraphType;

typedef struct MinHeapNode
{
    int vertex;
    int distance;
} MinHeapNode;

typedef struct MinHeap
{
    int size;
    int capacity;
    int* pos;
    MinHeapNode** array;
} MinHeap;

Node* create_node(int v, int weight)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode)
    {
        newNode->vertex = v;
        newNode->weight = weight;
        newNode->next = NULL;
    } 
    return newNode;
}

GraphType* create_graph(int n)
{
    GraphType* graph = (GraphType*)malloc(sizeof(GraphType));
    if (graph)
    {
        graph->n = n;

        for (int i = 0; i < n; i++)
        {
            graph->adj_list[i] = NULL;
        }
    }
   
    return graph;
}

void add_edge(GraphType* graph, int src, int dest, int weight)
{
    Node* newNode = create_node(dest, weight);
    newNode->next = graph->adj_list[src];
    graph->adj_list[src] = newNode;
}

MinHeapNode* create_min_heap_node(int v, int dist)
{
    MinHeapNode* minHeapNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    if (minHeapNode)
    {
        minHeapNode->vertex = v;
        minHeapNode->distance = dist;
    }  
    return minHeapNode;
}

MinHeap* create_min_heap(int capacity)
{
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    if (minHeap)
    {
        minHeap->pos = (int*)malloc(capacity * sizeof(int));
        minHeap->size = 0;
        minHeap->capacity = capacity;
        minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    }
    return minHeap;
}

void swap_min_heap_node(MinHeapNode** a, MinHeapNode** b)
{
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void min_heapify(MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->distance < minHeap->array[smallest]->distance)
    {
        smallest = left;
    }

    if (right < minHeap->size && minHeap->array[right]->distance < minHeap->array[smallest]->distance)
    {
        smallest = right;
    }

    if (smallest != idx)
    {
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->vertex] = idx;
        minHeap->pos[idxNode->vertex] = smallest;

        swap_min_heap_node(&minHeap->array[smallest], &minHeap->array[idx]);

        min_heapify(minHeap, smallest);
    }
}

int is_empty(MinHeap* minHeap)
{
    return minHeap->size == 0;
}

MinHeapNode* extract_min(MinHeap* minHeap)
{
    if (is_empty(minHeap))
    {
        return NULL;
    }

    MinHeapNode* root = minHeap->array[0];
    MinHeapNode* last_node = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = last_node;
    minHeap->pos[root->vertex] = minHeap->size - 1;
    minHeap->pos[last_node->vertex] = 0;
    --minHeap->size;
    min_heapify(minHeap, 0);

    return root;
}

void decrease_key(MinHeap* minHeap, int v, int dist)
{
    int i = minHeap->pos[v];
    minHeap->array[i]->distance = dist;

    while (i && minHeap->array[i]->distance < minHeap->array[(i - 1) / 2]->distance)
    {
        minHeap->pos[minHeap->array[i]->vertex] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->vertex] = i;
        swap_min_heap_node(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void print_path(int parent[], int j)
{
    if (parent[j] == -1)
    {
        return;
    }

    print_path(parent, parent[j]);
    printf("%d ", j);
}

void print_solution(int dist[], int parent[], int n, int src)
{
    printf("Distance\tPath");
    for (int i = 1; i < n; i++)
    {
        printf("\n%d\t\t", dist[i]);
        print_path(parent, i);
    }
}

void dijkstra(GraphType* graph, int src)
{
    int V = graph->n;
    int dist[MAX_VERTICES]{};
    int parent[MAX_VERTICES]{};

    MinHeap* minHeap = create_min_heap(V);

    for (int v = 0; v < V; ++v)
    {
        parent[src] = -1;
        dist[v] = INF;
        minHeap->array[v] = create_min_heap_node(v, dist[v]);
        minHeap->pos[v] = v;
    }

    dist[src] = 0;
    decrease_key(minHeap, src, dist[src]);

    minHeap->size = V;

    while (!is_empty(minHeap))
    {
        MinHeapNode* minHeapNode = extract_min(minHeap);
        int u = minHeapNode->vertex;

        Node* traverseNode = graph->adj_list[u];
        while (traverseNode != NULL)
        {
            int v = traverseNode->vertex;
            if (dist[u] != INF && traverseNode->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + traverseNode->weight;
                parent[v] = u;
                decrease_key(minHeap, v, dist[v]);
            }
            traverseNode = traverseNode->next;
        }
    }

    print_solution(dist, parent, V, src);
}

int main()
{
    int V = 10;
    GraphType* graph = create_graph(V);

    add_edge(graph, 0, 1, 3);
    add_edge(graph, 0, 5, 11);
    add_edge(graph, 0, 6, 12);
    add_edge(graph, 1, 0, 3);
    add_edge(graph, 1, 2, 5);
    add_edge(graph, 1, 3, 4);
    add_edge(graph, 1, 4, 1);
    add_edge(graph, 1, 5, 7);
    add_edge(graph, 1, 6, 8);
    add_edge(graph, 2, 1, 5);
    add_edge(graph, 2, 3, 2);
    add_edge(graph, 2, 6, 6);
    add_edge(graph, 2, 7, 5);
    add_edge(graph, 3, 1, 4);
    add_edge(graph, 3, 2, 2);
    add_edge(graph, 3, 4, 13);
    add_edge(graph, 3, 7, 14);
    add_edge(graph, 3, 9, 16);
    add_edge(graph, 4, 1, 1);
    add_edge(graph, 4, 3, 13);
    add_edge(graph, 4, 8, 18);
    add_edge(graph, 4, 9, 17);
    add_edge(graph, 5, 0, 11);
    add_edge(graph, 5, 1, 7);
    add_edge(graph, 5, 6, 9);
    add_edge(graph, 5, 9, 18);
    add_edge(graph, 6, 0, 12);
    add_edge(graph, 6, 1, 8);
    add_edge(graph, 6, 2, 6);
    add_edge(graph, 6, 5, 9);
    add_edge(graph, 6, 7, 13);
    add_edge(graph, 7, 2, 5);
    add_edge(graph, 7, 3, 14);
    add_edge(graph, 7, 6, 13);
    add_edge(graph, 7, 8, 15);
    add_edge(graph, 8, 4, 18);
    add_edge(graph, 8, 7, 15);
    add_edge(graph, 8, 9, 10);
    add_edge(graph, 9, 3, 16);
    add_edge(graph, 9, 4, 17);
    add_edge(graph, 9, 5, 18);
    add_edge(graph, 9, 8, 10);

    dijkstra(graph, 0);

    return 0;
}