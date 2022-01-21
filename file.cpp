/*
 * Authors: Gonçalo Nunes, Mateus Pinho
 * 
 * Description: Program to compute if a graph represents a geneologic tree,
 * where each vertex has, at most, 2 parents. It then finds all lowest common
 * ancestors for two given vertexes in the graph
 * 
 */


#include <vector>
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
#define RED_PATH 6


/* Represents a graph */
struct graph {
    std::vector<std::vector<int>> adjMatrix;
    size_t numberOfVertices = 0;
};


/* Transpose of the input graph, as the problem does not require the original
 * graph to be solved */
graph gt;

/* Colour vector to use in the Deph-First-Search and Breadth-First-Search algorithms */
std::vector<int> colour;

/* Prints the graph representation in an adjacency list representation
 * of the graph */
void printTable() {

    for (size_t v = 0; v < gt.numberOfVertices; v++) {
        printf("%ld : ", v + 1);
        for (int u: gt.adjMatrix[v]) {
            printf("%d | ", u);
        }
        printf("\n");
    }
}

void initializeGraphs(int numVertexes) {
    gt.adjMatrix = std::vector<std::vector<int>>(numVertexes);
    gt.numberOfVertices = numVertexes;
    colour.resize(numVertexes, WHITE);
}

void BFS(graph t, std::vector<int> &colour, int v) {

    /* Initialize queue */
    auto q = std::queue<int>();
    q.push(v);

    /* BFS main loop */
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        /* Iterate over adjacency list */
        for (int x: gt.adjMatrix[u - 1]) {

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
    /*
     * We define as a red path any path that starts on a vertex t, where
     * t is a lowest common ancestor for v and u and all other other vertexes
     * on that path are ancestors of t (descendants on the transpose graph)
     */

    /* Initialize queue */
    auto q = std::queue<int>();
    q.push(v);

    /* Check if v is itself a LCA */
    if (colour[v - 1] == BLACK) {
        colour[v - 1] = RED;
    }
    else {
        colour[v - 1] = BLUE;
    }

    /* BFS main loop */
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        /* Iterate over adjacency list */
        for (int x: gt.adjMatrix[u - 1]) {

            if (colour[x - 1] == RED && (colour[u - 1] == RED || colour[u - 1] == RED_PATH)) {
                /* If vertex X is actually an ancestor of another LCA candidate */
                colour[x - 1] = RED_PATH;
            }
            if (colour[x - 1] == WHITE) {
                /* Hasn't been explored yet and isn't an intersection
                 * point */
                colour[x - 1] = BLUE;
                q.push(x);
            }
            if (colour[x - 1] == BLUE && (colour[u - 1] == RED || colour[u - 1] == RED_PATH)) {
                /* If it is not an intersection point/LCA candidate and is
                 * part of a red path (is the ancestor of, at least, 1 LCA
                 * candidate) */
                colour[x - 1] = RED_PATH;
            }
            else if (colour[x - 1] == BLACK) {
                /* Vertex X is an LCA candidate */
                q.push(x);

                if (colour[u - 1] != RED && colour[u - 1] != RED_PATH) {
                    /* Wasn't discovered through a red path */
                    colour[x - 1] = RED;
                }
                else {
                    colour[x - 1] = RED_PATH;
                }

            }
        }
    }
}

int findAllLCA(int v1, int v2, std::vector<int> &LCAs) {
    // Find all LCA of v1 and v2

    size_t size = colour.size();
    for (size_t j = 0; j < size; j++) {
        /* Repaint colour vector */
        colour[j] = WHITE;
    }

    /* First BFS */
    BFS(gt, colour, v1);

    /* Second BFS */
    BFS2(gt, colour, v2);

    /* Populate array with the computed Lowest Common Ancestors for vertexes
     * v1 and v2 */
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

void processInput(std::vector<int> &firstLine) {
    size_t numVertexes, numEdges;
    int v1, v2;

    /* Read vertexes whose common ancestors are to be calculated */
    scanf("%d%d", &v1, &v2);
    firstLine[0] = v1;
    firstLine[1] = v2;

    /* Read number of vertexes and edges of the graph */
    scanf("%ld%ld", &numVertexes, &numEdges);

    /* Initialize graphs */
    initializeGraphs(numVertexes);

    for (size_t i = 0; i < numEdges; i++) {
        /* Read each edge from input */
        scanf("%d%d", &v1, &v2);
        
        /* Add edge to graph */
        gt.adjMatrix[v2 - 1].push_back(v1);

    }
}


int DFSVisit(graph &g, std::vector<int> &colour, int v) {
    int cycle = FALSE;
    colour[v - 1] = GRAY;

    for (int u: g.adjMatrix[v - 1]) {
        if (u == 0) {
            /* Reached end of v's adjacent vertexes */
            continue;
        }
        if (colour[u - 1] == GRAY) {
            /* Back edge found */
            return TRUE;
        }
        else if (colour[u - 1] == WHITE) {
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

    /* DFS Main cycle */
    for (size_t v = 0; v < gt.numberOfVertices; v++) {
        if (colour[v] == WHITE) {

            if (DFSVisit(gt, colour, v + 1) == TRUE) {
                return TRUE;
            }
        }
    }
    return FALSE; 
}

int isGeneologicTree(std::vector<int> &firstLine) {


    processInput(firstLine);

    size_t size = gt.adjMatrix.size();
    for (size_t i = 0; i < size; i++) {

        /* Go through adjacency list for each vertex. If any vertex
         * has an adjacency with more than MAX_NUMBER_OF_ANCESTORS, then it
         * cannot be a valid geneologic tree */
        if (gt.adjMatrix[i].size() > MAX_NUMBER_OF_ANCESTORS) {
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
    auto firstLine = std::vector<int>(2, 0);

    auto LCA = std::vector<int>();


    /* Check if it is a geneologic tree */
    isTree = isGeneologicTree(firstLine);

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
