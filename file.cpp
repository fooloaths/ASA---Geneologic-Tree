#include <vector>
// // #include <list>
#include <forward_list>
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
    // std::vector<std::list<int>> adjList;
    std::vector<std::vector<int>> adjMatrix;
    size_t numberOfVertices = 0;
};

graph gt;  // Transpose graph
std::vector<int> colour;


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
    //TODO maybe resize instead of new one?
    gt.adjMatrix = std::vector<std::vector<int>>(numVertexes, std::vector<int>(MAX_NUMBER_OF_ANCESTORS, 0));
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
            if (x == 0) {
                /* Reached end of adjacent vertexes for u */
                continue;
            }
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
    // // // auto lineage = std::vector<int>(gt.numberOfVertices, NIL);
    auto lineage = std::vector<bool>(gt.numberOfVertices, false);

    /* Initialize queue */
    auto q = std::queue<int>();
    q.push(v);

    /* Check if v is itself a LCA */
    if (colour[v - 1] == BLACK) {
        colour[v - 1] = RED;
        // // lineage[v - 1] = RED;
        lineage[v - 1] = true;
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
            // // // if (colour[x - 1] == RED && lineage[u - 1] == RED) {
            if (x == 0) {
                /* Reached end of adjacent vertexes for u */
                continue;
            }
            if (colour[x - 1] == RED && lineage[u - 1] == true) {
                /* If vertex X is actually an ancestor of another LCA */
                colour[x - 1] = BLUE;
                // // // lineage[x - 1] = RED;
                lineage[x - 1] = true;
            }
            if (colour[x - 1] == WHITE) {
                /* Hasn't been explored yet */
                colour[x - 1] = BLUE;
                q.push(x);
            }
            else if (colour[x - 1] == BLACK) {
                q.push(x);
                // // if (lineage[u - 1] == NIL) {
                if (lineage[u - 1] == false) {
                    colour[x - 1] = RED;
                }
                else {
                    colour[x - 1] = BLUE;
                }
                // // // lineage[x - 1] = RED;
                lineage[x - 1] = true;

            }
        }
    }
}

int findAllLCA(int v1, int v2, std::vector<int> &LCAs) {
    // Find all LCA of v1 and v2

    // // auto colour = std::vector<int>(gt.numberOfVertices, WHITE);
    size_t size = colour.size();
    for (size_t j = 0; j < size; j++) {
        /* Repaint colour vector */
        colour[j] = WHITE;
    }

    /* RE-SET COLOUR */

    /* First BFS */
    BFS(gt, colour, v1);

    /* Second BFS */
    BFS2(gt, colour, v2);

    // // size_t size = colour.size();
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

int processInput(std::vector<int> &firstLine) {
    size_t numVertexes, numEdges;
    int v1, v2;

    /* Read vertexes whose common ancestors are to be calculated */
    scanf("%d%d", &v1, &v2);
    firstLine[0] = v1;
    firstLine[1] = v2;
    /*firstLine.push_back(v1);
    firstLine.push_back(v2);*/

    /* Read number of vertexes and edges of the graph */
    scanf("%ld%ld", &numVertexes, &numEdges);

    /* Initialize graphs */
    initializeGraphs(numVertexes);

    for (size_t i = 0; i < numEdges; i++) {
        /* Read each edge from input */
        scanf("%d%d", &v1, &v2);
        
        // // // // printf("O v1 = %d e v2 = %d\n", v1, v2);
        /* Add edge to graph */
        if (gt.adjMatrix[v2 - 1][0] == 0) {
            gt.adjMatrix[v2 - 1][0] = v1;
        } 
        else if (gt.adjMatrix[v2 - 1][1] == 0) {
            gt.adjMatrix[v2 - 1][1] = v1;
        }
        else {
            /* Has more than two progenitors */
            printf("oooo\n");
            return FALSE;
        }

    }
    return TRUE;
}


int DFSVisit(graph &g, std::vector<int> &colour, int v) {
    int cycle = FALSE;
    colour[v - 1] = GRAY;

    // // // // printf("Estamos no vértice v = %d\nVamos ver as suas adjacências na DFSvisit", v);
    for (int u: g.adjMatrix[v - 1]) {
        // // // // printf("De momento temos u = %d\n", u);
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

    /* Setup DFS */
    // // // // std::vector<int> colour = std::vector<int>(gt.numberOfVertices, WHITE);
    
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

    int lessThanThreeParents = TRUE;

    lessThanThreeParents = processInput(firstLine);
    // // // printTable();

    if (lessThanThreeParents == FALSE) {
        // // // printf("aaaaaaaaaa\n");
        return FALSE;
    }
    // // size_t numVertexes = gt.numberOfVertices;
    // // for (size_t v = 0; v < numVertexes; v++) {
    // //     /* Iterate over vertexes */

    // //     /* Number of incoming edges in g = Number of outgoing edges in gt */
    // //     // // // int numEdges = gt.adjList[v].size();
    // //     //TODO change this
    // //     int numEdges = distance(gt.adjList[v].begin(), gt.adjList[v].end());
        
    // //     /* If the transpose graph has more outgoing edges than the maximum
    // //      * number of progenitors, then the graph cannot represent a 
    // //      * valid geneologic tree */
    // //     if (numEdges > MAX_NUMBER_OF_ANCESTORS) {
    // //         return FALSE;
    // //     }
    // // }

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
        return 0;
    }
    else {
        if ((hasLCA = findAllLCA(firstLine[0], firstLine[1], LCA))) {
            printLCAs(LCA);
        }
        else {
            printf("-\n");
        }
        return 0;
    }
    return 0;
}
