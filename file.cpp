#include <vector>
#include <list>
#include "stdio.h"

#define TRUE 1
#define FALSE 0
#define MAX_NUMBER_OF_ANCESTORS 2

struct graph {
    std::vector<std::list<int>> adjList;

};

graph g;   // Original graph
graph gt;  // Transpose graph

void printAdjList(std::list<int> l) {
    
    size_t size = l.size();
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


void initializeGraphs(int numVertexes) {
    printf("O tamanho da adjList era %d ", g.adjList.size());
    g.adjList.resize(numVertexes);
    gt.adjList.resize(numVertexes);
    printf("e agora é %d\n", g.adjList.size());
}

void processInput(std::vector<int> &firstLine) {
    int numVertexes, numEdges, v1, v2;

    /* Read vertexes whose common ancestors are to be calculated */
    printf("Insira os dois vértices cujos antepassados comuns devem ser calculados: ");
    scanf("%d%d", &v1, &v2);
    firstLine.push_back(v1);
    firstLine.push_back(v2);
    printf("Foram introduzidos os vértices %d e %d\n", v1, v2);

    /* Read number of vertexes and edges of the graph */
    printf("Introduza o número de vértices e o número de arcos da árvore: ");
    scanf("%d%d", &numVertexes, &numEdges);
    printf("O número de vértices é %d e o número de arcos é %d\n", numVertexes, numEdges);

    /* Initialize graphs */
    initializeGraphs(numVertexes);

    for (size_t i = 0; i < numEdges; i++) {
        /* Read each edge from input */
        scanf("%d%d", &v1, &v2);

        /* Add edge to original graph */
        g.adjList[v1 - 1].push_back(v2);
        
        /* Add edge to transpose graph */
        gt.adjList[v2 - 1].push_back(v1);
    }
}


int isGeneologicTree() {

    size_t numVertexes = gt.adjList.size();
    for (size_t v = 0; v < numVertexes; v++) {
        //Iterate over vertexes

        /* Number of incoming edges in g/ outgoing edges in gt */
        int numEdges = gt.adjList[v].size();
        
        /* If the transpose graph has more outgoing edges than the maximum
         * number of progenitors, then the graph cannot represent a 
         * valid geneologic tree */
        if (numEdges > MAX_NUMBER_OF_ANCESTORS) {
            return FALSE;
        }
    }

    /* Check for cycles */

    return TRUE;
}

int main() {
    int isTree;
    std::vector<int> firstLine = std::vector<int>(); // Vector with the 2 vertexes
                                                    // Whose common ancestor are to be calculated


    /* Proccess input */
    processInput(firstLine);

    /* Check if it is a geneologic tree */
    isTree = isGeneologicTree();

    if (isTree == FALSE) {
        printf("0");
    }
    else {
        //Part 2
    }


    return 0;
}


