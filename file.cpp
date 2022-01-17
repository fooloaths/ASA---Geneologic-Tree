#include <vector>
#include <list>
#include <queue>
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


/* Represents a graph */
struct graph {
    std::vector<std::list<int>> adjList;
};


graph gt;  // Transpose graph

void BFS(graph t, std::vector<int> &colour, int v) {

    /* Initialize queue */
    auto q = std::queue<int>();
    q.push(v);

    /* BFS main loop */
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        /* Iterate over adjacency list */
        for (int x: gt.adjList[u - 1]) {
            if (colour[x - 1] == WHITE) {
                /* Hasn't been explored yet */
                colour[x - 1] = GRAY;
                q.push(x);
            }
        }
        colour[u - 1] = BLACK;
    }
}

void BFS2(graph t, std::vector<int> &colour, int v) {

    /* Auxiliary vectors */
    auto lineage = std::vector<int>(gt.adjList.size(), NIL);

    /* Initialize queue */
    auto q = std::queue<int>();
    q.push(v);

    /* Check if v is itself a LCA */
    if (colour[v - 1] == BLACK) {
        colour[v - 1] = RED;
        lineage[v - 1] = RED;
    }
    else {
        colour[v - 1] = BLUE;
    }

    /* BFS main loop */
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        /* Iterate over adjacency list */
        for (int x: gt.adjList[u - 1]) {
            if (colour[x - 1] == RED && lineage[u - 1] == RED) {
                /* If vertex X is actually an ancestor of another LCA */
                colour[x - 1] = BLUE;
                lineage[x - 1] = RED;
            }
            if (colour[x - 1] == WHITE) {
                /* Hasn't been explored yet */
                colour[x - 1] = BLUE;
                q.push(x);
            }
            else if (colour[x - 1] == BLACK) {
                q.push(x);
                if (lineage[u - 1] == NIL) {
                    colour[x - 1] = RED;
                }
                else {
                    colour[x - 1] = BLUE;
                }
                lineage[x - 1] = RED;

            }
        }
    }
}

int findAllLCA(int v1, int v2, std::vector<int> &LCAs) {
    // Find all LCA of v1 and v2

    auto colour = std::vector<int>(gt.adjList.size(), WHITE);

    /* First BFS */
    BFS(gt, colour, v1);

    /* Second BFS */
    BFS2(gt, colour, v2);

    size_t size = colour.size();
    for (size_t i = 0; i < size; i++) {
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
    gt.adjList.resize(numVertexes);
}

void processInput(std::vector<int> &firstLine) {
    size_t numVertexes, numEdges;
    int v1, v2;

    /* Read vertexes whose common ancestors are to be calculated */
    scanf("%d%d", &v1, &v2);
    firstLine.push_back(v1);
    firstLine.push_back(v2);

    /* Read number of vertexes and edges of the graph */
    scanf("%ld%ld", &numVertexes, &numEdges);

    /* Initialize graphs */
    initializeGraphs(numVertexes);

    for (size_t i = 0; i < numEdges; i++) {
        /* Read each edge from input */
        scanf("%d%d", &v1, &v2);
        
        /* Add edge to transpose graph */
        gt.adjList[v2 - 1].push_back(v1);
    }
}


int DFSVisit(graph &g, std::vector<int> &colour, int v) {
    int cycle = FALSE;
    colour[v - 1] = GRAY;

    for (int u: g.adjList[v - 1]) {
        if (colour[u - 1] == GRAY) {
            /* Back edge found */
            return TRUE;
        } else if (colour[u - 1] == WHITE) {
            cycle = DFSVisit(g, colour, u);
            
            if (cycle == TRUE) {
                return TRUE;
            }
        }
    }
    colour[v - 1] = BLACK;
    return FALSE;
}

int DFSCycleDetection() {

    /* Setup DFS */
    std::vector<int> colour = std::vector<int>(gt.adjList.size(), WHITE);
    
    /* DFS Main cycle */
    for (size_t v = 0; v < gt.adjList.size(); v++) {
        if (colour[v] == WHITE) {

            if (DFSVisit(gt, colour, v + 1) == TRUE) {
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
    int isTree  = TRUE, hasLCA = 0;

    /* Vector with the 2 vertexes
     * Whose common ancestor are to be calculated */
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
        if ((hasLCA = findAllLCA(firstLine[0], firstLine[1], LCA))) {
            printLCAs(LCA);
        }
        else {
            printf("-\n");
        }
    }

    return 0;
}
