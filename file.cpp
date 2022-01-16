#include <vector>
#include <list>
#include <queue>
#include <stack>
// #include <iterator>
#include "stdio.h"

#define TRUE 1
#define FALSE 0
#define MAX_NUMBER_OF_ANCESTORS 2
#define WHITE 1
#define GRAY 2
#define BLACK 3
#define RED 4
#define BLUE 5
#define NIL -1
#define NOT_VISITED -1
#define IN_STACK 1

//TODO stack can be replace by vector


/* Represents a graph */
struct graph {
    std::vector<std::list<int>> adjList;
};

//get vertex list 

graph g;   // Original graph
graph gt;  // Transpose graph

void printAdjList(std::list<int> l) {
    
    for (int v: l) {
        printf("[%d]->", v);
    }
}

void printGraph(graph t) {

    size_t numVertexes = t.adjList.size();
    for (size_t v = 0; v < numVertexes; v++) {
        printf("%ld | ", v + 1);
        printAdjList(t.adjList[v]);
        printf("\n");
    }
}

void BFS(graph t, std::vector<int> &colour, std::vector<int> &BFSTree, int v) {

    /* Initialize queue */
    auto q = std::queue<int>();
    q.push(v);

    /* Set v as root of the BFS tree */
    BFSTree[v - 1] = 0;

    /* BFS main loop */
    int level = 1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        /* Iterate over adjacency list */
        for (int x: gt.adjList[u - 1]) {
            if (colour[x - 1] == WHITE) {
                /* Hasn't been explored yet */
                colour[x - 1] = GRAY;
                BFSTree[x - 1] = level;
                q.push(x);
            }
        }
        level++;
        colour[u - 1] = BLACK;
    }
}

void BFS2(graph t, std::vector<int> &colour, std::vector<int> &BFSTree, int v) {

    /* Auxiliary vectors */
    auto lineage = std::vector<int>(gt.adjList.size(), NIL);
    auto predecessor = std::vector<int>(gt.adjList.size(), NIL);

    /* Initialize queue */
    auto q = std::queue<int>();
    q.push(v);

    /* Set v as root of the BFS tree */
    BFSTree[v - 1] = 0;

    /* BFS main loop */
    int level = 1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        /* Iterate over adjacency list */
        for (int x: gt.adjList[u - 1]) {
            if (colour[x - 1] == WHITE) {
                /* Hasn't been explored yet */
                colour[x - 1] = BLUE;
                BFSTree[x - 1] = level;
                q.push(x);
                predecessor[x - 1] = u;
            }
            else if (colour[x - 1] == BLACK) {
                BFSTree[x - 1] = level;
                q.push(x);
                predecessor[x - 1] = u;
                if (lineage[u - 1] == NIL) {
                    colour[x - 1] = RED;
                }
                else {
                    colour[x - 1] = BLUE;
                }
                lineage[x - 1] = RED;

            }
        }
        level++;
        // // colour[u - 1] = BLACK;
    }
}

int findAllLCA(int v1, int v2, std::vector<int> &LCAs) {
    // Find all LCA of v1 and v2

    auto colour = std::vector<int>(gt.adjList.size(), WHITE);
    auto v1BFSTree = std::vector<int>(gt.adjList.size(), NIL);
    auto v2BFSTree = std::vector<int>(gt.adjList.size(), NIL);

    // printf("11111\n");
    /* First BFS */
    printGraph(gt);
    /*printf("Os vértices a comparar são o %d e %d\n", v1, v2);
    for (int u: gt.adjList[v1 - 1]) {
        printf("%d --- ", u);
    } printf("\n");*/
    BFS(gt, colour, v1BFSTree, v1);
    // printf("22222\n");
    /* Second BFS */
    BFS2(gt, colour, v2BFSTree, v2);
    // printf("33333\n");

    size_t size = colour.size();
    // printf("Colour:\n");
    // for (size_t j = 0; j < size; j++) {
    //     printf("\tVértice %ld = ", j + 1);
    //     if (colour[j] == RED) {
    //         printf("RED\n");
    //     }
    //     else if (colour[j] == BLUE) {
    //         printf("BLUE\n");
    //     }
    //     else if (colour[j] == BLACK) {
    //         printf("BLACK\n");
    //     }
    //     else if (colour[j] == WHITE) {
    //         printf("WHITE\n");
    //     }
    // }
    for (size_t i = 0; i < size; i++) {
        // printf("44444\n");
        if (colour[i] == RED) {
            LCAs.push_back(i + 1);
        }
    }


    return LCAs.size() > 0;
}

void printLCAs(std::vector<int> &LCA) {
    for (int v: LCA) {
        printf("%d ", v);
    }
    printf("\n");
}

void initializeGraphs(int numVertexes) {
    // // printf("O tamanho da adjList era %d ", g.adjList.size());
    g.adjList.resize(numVertexes);
    gt.adjList.resize(numVertexes);
    // // printf("e agora é %d\n", g.adjList.size());
}

void processInput(std::vector<int> &firstLine) {
    size_t numVertexes, numEdges;
    int v1, v2;
    int scanfResult;
    /* Read vertexes whose common ancestors are to be calculated */
    // // printf("Insira os dois vértices cujos antepassados comuns devem ser calculados: ");
    scanfResult = scanf("%d%d", &v1, &v2);
    firstLine.push_back(v1);
    firstLine.push_back(v2);
    // // printf("Foram introduzidos os vértices %d e %d\n", v1, v2);

    /* Read number of vertexes and edges of the graph */
    // // printf("Introduza o número de vértices e o número de arcos da árvore: ");
    scanfResult = scanf("%ld%ld", &numVertexes, &numEdges);
    // // printf("O número de vértices é %d e o número de arcos é %d\n", numVertexes, numEdges);

    /* Initialize graphs */
    initializeGraphs(numVertexes);

    for (size_t i = 0; i < numEdges; i++) {
        /* Read each edge from input */
        scanfResult = scanf("%d%d", &v1, &v2);

        /* Add edge to original graph */
        g.adjList[v1 - 1].push_back(v2);
        
        /* Add edge to transpose graph */
        gt.adjList[v2 - 1].push_back(v1);
    }

    /*printf("###### Original Graph ######\n");
    printGraph(g);
    printf("###### Transpose Graph ######\n");
    printGraph(gt);*/
}

int DFSVisit(graph g, std::stack<int> q, std::vector<int> visited) {
    for (int v: g.adjList[q.top() - 1]) {
        if (visited[v - 1] == IN_STACK) {
            return TRUE;
        } else if (visited[v - 1] == NOT_VISITED) {
            q.push(v - 1);
            visited[v - 1] = IN_STACK;
            DFSVisit(g, q, visited);
        }
    }
    visited[q.top() - 1] = NIL;
    q.pop();
    return FALSE;
}

int DFSCycleDetection() {

    /* Setup DFS */
    std::vector<int> visited = std::vector<int>(g.adjList.size(), NIL);
    
    /* DFS Main cycle */
    for (int v = 0; v < g.adjList.size(); v++) {
        if (visited[v - 1] == NOT_VISITED) {
            std::stack<int> q;
            q.push(v);
            visited[v - 1] = IN_STACK;
            if (DFSVisit(g, q, visited) == IN_STACK) {
                return TRUE;
            }
        }
    }
    return FALSE; 
}


int isGeneologicTree() {

    size_t numVertexes = gt.adjList.size();
    for (size_t v = 0; v < numVertexes; v++) {
        /* Iterate over vertexes */

        /* Number of incoming edges in g = Number of outgoing edges in gt */
        int numEdges = gt.adjList[v].size();
        
        /* If the transpose graph has more outgoing edges than the maximum
         * number of progenitors, then the graph cannot represent a 
         * valid geneologic tree */
        if (numEdges > MAX_NUMBER_OF_ANCESTORS) {
            return FALSE;
        }
    }

    /* Check for cycles */
    if (DFSCycleDetection() == TRUE) {
        return FALSE;
    }

    return TRUE;
}

int main() {
    int isTree, hasLCA = 0;
    // Vector with the 2 vertexes
    // Whose common ancestor are to be calculated
    auto firstLine = std::vector<int>(); 
    auto LCA = std::vector<int>();

    /* Proccess input */
    processInput(firstLine);

    /* Check if it is a geneologic tree */
    isTree = isGeneologicTree();

    if (isTree == FALSE) {
        printf("0\n");
    }
    else {
        if (hasLCA == findAllLCA(firstLine[0], firstLine[1], LCA)) {
            printLCAs(LCA);
        }
        else {
            printf("-\n");
        }
    }

    return 0;
}
