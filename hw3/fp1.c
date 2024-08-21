#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTEX 10000
#define MAX_EDGE 50000000

clock_t start, finish;
double duration;

// 간선을 나타내는 구조체
typedef struct {
    int source;
    int destination;
    int weight;
} Edge;

// 그래프의 구조체
typedef struct {
    int V;          // 정점의 수
    int E;          // 간선의 수
    Edge* edges;    // 간선들의 배열
} Graph;

// 분리 집합의 노드를 나타내는 구조체
typedef struct {
    int parent;
    int rank;
} Node;

int V; // 정점의 수
int E; // 간선의 수

Graph* createGraph(int V, int E);
Node* createSet(int V);
void insertEdgeToMinHeap(Edge* minHeap, int* heapSize, Edge e);
Edge extractMinFromHeap(Edge* minHeap, int* heapSize);
int find(Node* set, int i);
void unionSets(Node* set, int x, int y);
void kruskalMST(Graph* graph);
void printOutput(Edge* result, int resultIndex);
void destroyGraph(Graph* graph);

int main(int argc, char* argv[]) {
    start = clock();

    // If the number of command-line arguments is not 1 (zero or more than 1)
    if(argc != 2) {
        printf("usage: ./fp1 input_filename\n");
        exit(1);
    }

    // 파일 열기
    FILE *input;
	input = fopen(argv[1], "r");

	// If the input file does not exist
	if(input == NULL) {
		printf("The input file does not exist.\n");
		exit(1);
	}

    fscanf(input, "%d", &V);
    fscanf(input, "%d", &E);

    int read;
    int s, d, w;
    int idx = 0;

    Graph* graph = createGraph(V, E);

    while(1) {
        // 명령문 한 줄씩 읽어오기
        read = fscanf(input, "%d %d %d", &s, &d, &w);

        // input 파일 다 읽으면 루프 종료
        if(read == EOF) break;

        // 간선 정보 추가
        graph->edges[idx].source = s;
        graph->edges[idx].destination = d;
        graph->edges[idx].weight = w;

        idx++;
    }

    fclose(input);

    kruskalMST(graph);

    destroyGraph(graph);

    printf("output written to fp1_result.txt.\n");
    finish = clock();
    duration = (double)(finish-start)/CLOCKS_PER_SEC;
    printf("running time: %.6lf seconds\n", duration);

    return 0;
}

// 그래프 생성
Graph* createGraph(int V, int E) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edges = (Edge*)malloc(E * sizeof(Edge));
    return graph;
}

// 분리 집합 생성
Node* createSet(int V) {
    Node* set = (Node*)malloc(V * sizeof(Node));
    for (int i = 0; i < V; i++) {
        set[i].parent = i;
        set[i].rank = 0;
    }
    return set;
}

// 간선을 최소 힙에 삽입
void insertEdgeToMinHeap(Edge* minHeap, int* heapSize, Edge e) {
    int i = ++(*heapSize);
    // printf("%d %d %d\n", e.source, e.destination, e.weight);

    while((i != 1) && (e.weight < minHeap[i/2].weight)) {
        minHeap[i] = minHeap[i/2];
        i /= 2;
    }
    minHeap[i] = e;
}


// 최소 힙에서 최소값 추출
Edge extractMinFromHeap(Edge* minHeap, int* heapSize) {
    int parent, child;
    Edge item, temp;
    // save value of the element with the lowest weight
    item = minHeap[1];
    // use the last element in the heap to adjust heap
    temp = minHeap[(*heapSize)--];
    parent = 1;
    child = 2;

    while(child <= (*heapSize)) {
        // find the smaller child of the current parent 
        if((child < (*heapSize)) && (minHeap[child].weight > minHeap[child+1].weight)) child++;
        if(temp.weight <= minHeap[child].weight) break;
        // move to the next lower level
        minHeap[parent] = minHeap[child];
        parent = child;
        child *= 2;
    }
    minHeap[parent] = temp;
    return item;
}

// 분리 집합의 대표 노드 찾기
int find(Node* set, int i) {
    if (set[i].parent != i)
        set[i].parent = find(set, set[i].parent);
    return set[i].parent;
}

// 분리 집합의 두 집합을 합침
void unionSets(Node* set, int x, int y) {
    int xRoot = find(set, x);
    int yRoot = find(set, y);

    if (set[xRoot].rank < set[yRoot].rank)
        set[xRoot].parent = yRoot;
    else if (set[xRoot].rank > set[yRoot].rank)
        set[yRoot].parent = xRoot;
    else {
        set[yRoot].parent = xRoot;
        set[xRoot].rank++;
    }
}

// 크루스칼 알고리즘을 이용한 최소 신장 트리 생성
void kruskalMST(Graph* graph) {
    int V = graph->V;
    int E = graph->E;
    Edge* result = (Edge*)malloc(MAX_VERTEX * sizeof(Edge));
    Edge* minHeap = (Edge*)malloc(MAX_EDGE * sizeof(Edge));
    int heapSize = 0;
    int resultIndex = 0;
    int i = 0;

    // 그래프의 간선들을 최소 힙에 삽입
    for (i = 0; i < E; i++)
        insertEdgeToMinHeap(minHeap, &heapSize, graph->edges[i]);

    Node* set = createSet(V);
    
    // 최소 힙에서 간선 추출하여 검사
    while (resultIndex < V - 1 && heapSize != 0) {
        Edge minEdge = extractMinFromHeap(minHeap, &heapSize);
        int sourceRoot = find(set, minEdge.source);
        int destRoot = find(set, minEdge.destination);

        // 사이클이 생기지 않는 경우에만 간선을 선택
        if (sourceRoot != destRoot) {
            result[resultIndex++] = minEdge;
            unionSets(set, sourceRoot, destRoot);
        }
    }

    printOutput(result, resultIndex);

    free(result);
    free(minHeap);
    free(set); 
}

void printOutput(Edge* result, int resultIndex) {
    FILE *output;
	output = fopen("fp1_result.txt", "w");
    int sum = 0;

    for (int i = 0; i < resultIndex; i++) {
        fprintf(output, "%d %d %d\n", result[i].source, result[i].destination, result[i].weight);
        sum += result[i].weight;
    }

    fprintf(output, "%d\n", sum);

    if(resultIndex < V - 1) fprintf(output, "DISCONNECTED");
    else fprintf(output, "CONNECTED");

    fclose(output);
}

// 메모리 해제
void destroyGraph(Graph* graph) {
    free(graph->edges);
    free(graph);
}