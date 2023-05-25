#include "Graph.h"
#include "List.h"
#include "PQueue.h"
#include "Dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Point {
    int x;
    int y;
} Point;

Point *create_point(int x, int y) {
    Point *p = malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}


// Returns 0 if points are equal, -1 otherwise
int PointsEqual(void* point1, void* point2) {
    Point* p1 = (Point*)point1;
    Point* p2 = (Point*)point2;

    if(p1->x == p2->x && p1->y == p2->y) {
        return 0;
    }
    else {
        return -1;
    }
}

void PrintPoint(void* pt) {
    Point* p1 = (Point*)pt;
    printf("%d %d\n", p1->x, p1->y);
}

// free point
void DestroyPoint(Point* pt) {
    free(pt);
}

// int set of helpers
int ICompare(void* obj1, void* obj2) {
    int* v1 = (int*)obj1;
    int* v2 = (int*)obj2;

    if(*v1 == *v2) {
        return 0;
    }
    else {
        return -1;
    }
}

void IPrinter(void* data) {
    int* v1 = (int*)data;
    printf("%d ", *v1);
}

void IFree(void* data) {
    int* dat = (int*)data;
    free(dat);
}

/**
 * @brief Implements the bst algorithm to find the shortest path between 2 nodes in an unweighted graph.
 * 
 * @param g The graph to search through
 * @param vertex_start The index of the vertex in the graph from where to start the search.
 * @param vertex_end The index of the vertex in the graph that needs to be reached.
 * @param vertices_explored The value in this pointer should be set to the number of vertices that were visited by the algorithm.
 * @return ListPtr A list of the nodes that make up the path from the start to the end, including the start and end nodes.
 */
ListPtr bfs(Graph *g, int vertex_start, int vertex_end, long *vertices_explored) {
    //Perform bfs traversal. Make sure to store who the parent of each node in your traversal is.
    // Mark visited nodes to maek sure not to visit them twice
    ListPtr frontier = list_create(&ICompare, &IPrinter, &IFree);
    int begin = vertex_start;
    int vindex = 0;
    int* parents = (int*)malloc(sizeof(int) * graph_num_vertices(g)); // parents array that keeps track of where each index originated from. Example: if node 3 leads to node 2, then parents[2] = 3.
    for(unsigned i = 0; i < graph_num_vertices(g); i++) { // set default values
        parents[i] = -2;
    }

    parents[vertex_start] = -1; // vertex_start has no parent

    if(graph_num_vertices(g) < vertex_start) {
        return NULL;
    }
    int* v = malloc(sizeof(int));
    *v = vertex_start;
    list_append(frontier, v); // put first vertex into the frontier

    while(list_length(frontier) != 0) {
        int* current = list_del_index(frontier, 0); // pop out of frontier and assign to current vertex visiting
        int vertex = *current;

        if(vertex == vertex_end) {
            break;
        }

        ListPtr neighbors = graph_get_neighbors(g, vertex); // returns listptr of neighbors surrounding vertex


        for(unsigned int i = 0; i < list_length(neighbors); i++) { // for each neighbor, check if vertex has been visited
            int* next = list_get_index(neighbors, i);
            if(parents[*next] == -2) { // if the vertex has not been visited, visit and queue up in frontier, and insert into parents array
                list_append(frontier, next);
                parents[*next] = vertex;
                vindex++;
            }
        }

    }

    ListPtr path = list_create(&ICompare, &IPrinter, &IFree);
    int trace_back = vertex_end;

    while(trace_back != vertex_start) { // traceback ends when vertex_end = vertex_start
        int *temp = (int*)malloc(sizeof(int));
        *temp = trace_back;
        list_insert(path, 0, temp); // we use insert rather than append to make sure the path is in the correct order as we are tracing backwards.
        trace_back = parents[trace_back];
    }
    int *temp = (int*)malloc(sizeof(int)); // iterate once more to get the last vertex (at vertex_start)
    *temp = trace_back;
    list_insert(path, 0, temp);

    *vertices_explored = vindex; // return vertices_explored
    return path;
}

/**
 * @brief Implements the A* search algorithm to find the shortest path between 2 nodes in an unweighted graph.
 * 
 * @param g The graph to search through
 * @param nodeArr Array of positions of the graph vertices on a 2d plane. Note this is a pointer to a contiguous array, where nodeArr[i] should correspond to node i in the graph. 
 * @param vertex_start The index of the vertex in the graph from where to start the search.
 * @param vertex_end The index of the vertex in the graph that needs to be reached.
 * @param vertices_explored The value in this pointer should be set to the number of vertices that were visited by the algorithm.
 * @return ListPtr A list of the nodes that make up the path from the start to the end, including the start and end nodes.
 */
ListPtr a_star(Graph *g, Point *nodeArr, int vertex_start, int vertex_end, long *vertices_explored) {
    //Perform a star traversal. Make sure to store who the parent of each node in your traversal is, and the path cost up to that node.
    return NULL;
}


int main(int argc, char* argv[]) {
    // argv[1] will tell you if to run bfs or a_star
    int algorithm = (strcmp(argv[1], "bfs") == 0) ? 0 : 1;
    // stdin will contain your input


    //Parse node locations + count number of nodes

    // Create graph

    //Parse and add graph edges

    //Parse start and end points and run through correct search algorithm
    //Print results to stdout

    //free Data structures

    char nodes[BUFSIZ] = ""; // stores --nodes--
    char line[BUFSIZ] = "";
    char delim[BUFSIZ] = " \n";
    int numNodes = 0;
    int var1 = 0, var2 = 0;
    ListPtr nodelist = list_create(PointsEqual, PrintPoint, DestroyPoint);


    fgets(nodes, sizeof(nodes), stdin); // Taking ==== Nodes ====

    while(fgets(line, BUFSIZ, stdin)) { // read in Nodes, skip title text
        if(strcmp(line, "======= Edges =======\n") == 0) {
            break;
        }
        sscanf(line, "%d %d", &var1, &var2); // reads in node located at (x, y) = (var1, var2)
        Point* coord = create_point(var1, var2);
        list_append(nodelist, coord);
        numNodes++;
    }

    Graph* g = graph_create(numNodes); // create graph given numNodes
    var1 = 0;
    var2 = 0;

    while(fgets(line, BUFSIZ, stdin)) { // read in Edges, skip title text
        if(strcmp(line, "======= Find paths =======\n") == 0) {
            break; 
        }
        sscanf(line, "%d %d", &var1, &var2);
        graph_add_edge(g, var1, var2);
    }



    var1 = 0;
    var2 = 0;
    ListPtr startlist = list_create(ICompare, IPrinter, IFree);
    ListPtr endlist = list_create(ICompare, IPrinter, IFree);


    while(fgets(line, BUFSIZ, stdin)) { // read in Paths to find until EOF.
        sscanf(line, "%d %d", &var1, &var2);
        list_append(startlist, var1);
        list_append(endlist, var2);
    }

    long* vertices_explored = (long*)malloc(sizeof(long));
    ListPtr path = list_create(ICompare, IPrinter, IFree);

    for(unsigned int i = 0; i < list_length(startlist); i++) {
        
        int* vertex_start = list_get_index(startlist, i);
        int* vertex_end = list_get_index(endlist, i);

        if(algorithm == 0) {
            ListPtr path = bfs(g, vertex_start, vertex_end, vertices_explored);

            printf("\nPath: ");
            list_print(path);
            printf("\n");


            // from here, we have the list of nodes that the path took to reach the end goal. We now need to output our data in the format:
            // number_explored path_length left_steps right_steps up_steps down_steps.
            // we can find these values thru vertices_explored, list_length of path, and determining the coordinate difference between each node within the path list and grabbing the coordinate information from the node list we read in from the .in file.
            //list_print(path);

            int left_steps = 0, right_steps = 0, up_steps = 0, down_steps = 0;

            for(unsigned int i = 0; i < list_length(path) - 1; i++) {
                // in order to get the steps from start to end, we take the int indexes stored in path's first two indices, then use those int indexes to navigate to the nodes list to get the coordinates

                // example: *vertex1 = 198, *vertex2 = 4; we check for Point* temp = list_get_index(nodelist, vertex1/vertex2); these will return points. 
                // if v1->x - v2->x = 0; then that means there is no movement either left or right. if v1->y - v2->y = 0; there is no movement up or down.
                // if v1->x - v2->x > 0; then that means the movement is towards the left. If v1->x - v2->x < 0; then that means the movement is towards the right.
                // if v1->y - v2->y > 0; then that means the movement is upwards. if v1->y - v2->y < 0; then that means the movement is downwards. (cairo)
                int* vertex1 = list_get_index(path, i);
                int* vertex2 = list_get_index(path, i + 1);

                Point* temp1 = create_point(0, 0);
                Point* temp2 = create_point(0, 0);

                temp1 = list_get_index(nodelist, *vertex1);
                temp2 = list_get_index(nodelist, *vertex2);

                if(temp2 != NULL) {
                    if(temp1->x - temp2->x > 0) {
                        left_steps++;
                    }
                    if(temp1->x - temp2->x < 0) {
                        right_steps++;
                    }
                    if(temp1->y - temp2->y > 0) {
                        up_steps++;
                    }
                    if(temp1->y - temp2->y < 0) {
                        down_steps++;
                    }
                }

            }

            printf("%ld %d %d %d %d %d\n", *vertices_explored, list_length(path) - 1, left_steps, right_steps, up_steps, down_steps);
        }
        else { // a_star implementation.

        }
    }
    return 0;
}