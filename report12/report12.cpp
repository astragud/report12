// report12.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTICES 20
#define INF 100

typedef struct GraphType
{
	int n; // 정점의 개수
	int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;

int distance[MAX_VERTICES]; //시작정점으로부터의 최단경로 거리
int found[MAX_VERTICES]; //방문한 정점 표시 

int choose(int distance[], int n, int found[])
{
	int min = INF, minpos = -1;
	for (int i = 0; i < n; i++)
		if (distance[i] < min && !found[i])
		{
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

void print_status(GraphType* g)
{
	printf("distance: ");

	for (int i = 0; i < g->n; i++)
	{
		if (distance[i] == INF)
		{
			printf(" * ");
		}
		
		else
		{
			printf("%d ", distance[i]);
		}
	}

	printf("\n");
	printf("found: ");
	for (int i = 0; i < g->n; i++)
	{
		printf("%d ", found[i]);
	}
	printf("\n\n");
}

void shortest_path(GraphType* g, int start)
{
	int i, u, w;
	for (i = 0; i < g->n; i++) //초기화
	{
		distance[i] = g->weight[start][i];
		found[i] = 0;
	}

	found[start] = 1; //시작 정점 방문 표시
	distance[start] = 0;
	int shortest[MAX_VERTICES]{};

	printf("Dijkstra Algorithm\n");
	for (i = 0; i < g->n - 1; i++)
	{
		print_status(g);
		u = choose(distance, g->n, found);
		found[u] = 1;
		
		for (w = 0; w < g->n; w++)
		{
			if (!found[w])
			{
				if (distance[u] + g->weight[u][w] < distance[w])
				{
					distance[w] = distance[u] + g->weight[u][w];
				}
			}
		}
		shortest[i] = u;
	}

	for (int i = 0; i < g->n; i++)
	{
		printf("%d ", shortest[i]);
	}
}


int main()
{
	GraphType g = { 10,
		{{ 0, 3, INF, INF, INF, 11, 12, INF, INF, INF },
		{ 3, 0, 5, 4, 1, 7, 8, INF, INF, INF },
		{ INF, 5, 0, 2, INF, INF, 6, 6, INF, INF },
		{ INF, 4, 2, 0, 13, INF, INF, 14, INF, 16 },
		{ INF, 1, INF, 13, 0, 9, INF, INF, 18, 17 },
		{ 11, 7, INF, INF, 9, 0, INF, INF, INF, INF },
		{ 12, 8, 6, INF, INF, INF, 0, 13, INF, INF },
		{ INF, INF, 5, 14, INF, INF, 13, 0, INF, 15 },
		{ INF, INF, INF, INF, 18, INF, INF, INF, 0, 10 },
		{ INF, INF, INF, 16, 17, INF, INF, 15, 10, 0 }}
	};

	shortest_path(&g, 0);
	return 0;
}