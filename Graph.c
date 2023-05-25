#include <stdlib.h>
#include <stdio.h>
#include "List.h"
#include "Graph.h"

typedef struct Graph{
    int num_vertices;
    ListPtr *adjacency_list; // Is an array of ListPtrs for faster access
} Graph;

// Hints: You will need to implement a ListPtr DataCompare, DataPrinter and 
//        freeData for the lists used in the adjacency list. These will be ints.
//        You can copy this from the ones in prog5.c, but they should have different name to avoid a namesapce collision.
//TODO: Implement all functions from the header file.

int VCompare(void* obj1, void* obj2) {
    int* v1 = (int*)obj1;
    int* v2 = (int*)obj2;

    if(*v1 == *v2) {
        return 0;
    }
    else {
        return -1;
    }
}

void VPrinter(void* data) {
    int* v1 = (int*)data;
    printf("%d\n", v1);
}

void VFree(void* data) {
    int* dat = (int*)data;
    free(dat);
}

Graph *graph_create(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_vertices = numVertices;
    graph->adjacency_list = (ListPtr*)calloc(graph->num_vertices, sizeof(ListPtr)); // instantiate an array of lists, then list_create at each pointer.

    for(unsigned int i = 0; i < graph->num_vertices; i++) { // instantiate list within each index
        graph->adjacency_list[i] = list_create(VCompare, VPrinter, (void*)VFree);
    }

    return graph;
}

void graph_destroy(Graph *g) {
    for(unsigned int i = 0; i < g->num_vertices; i++) {
        list_destroy(g->adjacency_list[i], true); // destroy graph always frees data within the lists
    }
}

bool graph_add_edge(Graph *g, int v1, int v2) { // int v2 is the index of the list, v2 is the data field of linked list object within that index
    // add edge to a graph given that the two vertices exist
    // if out of bounds -> if v1 is greater than the number of vertices available, return false
    // if edge already exists -> if list_find_index(g->adj_list[v1], v2) returns != NULL, return false
    // if neither are true, list_append(g->adj_list[v1], 2).
    int* temp1 = (int*)malloc(sizeof(int));
    *temp1 = v1;
    int* temp2 = (int*)malloc(sizeof(int));
    *temp2 = v2;

    if(v1 >= g->num_vertices || (list_get_index(g->adjacency_list[*temp1], temp2) != NULL && list_get_index(g->adjacency_list[*temp2], temp1) != NULL)) {
        printf("Could not add edge.\n");
        return false;
    }
    else {
        list_append(g->adjacency_list[*temp1], temp2);
        list_append(g->adjacency_list[*temp2], temp1);
        return true;
    }
}

bool graph_add_nodes(Graph *g, int numVertices) {
    // resize graph to account for numVertices more indices.
    // realloc to new size (oldsize + newsize) * sizeof(listPtr) -> g->num_vertices + numVertices * sizeof(ListPtr)
    // condition failing realloc
    //if(true) { // figure out c
    int osize = g->num_vertices;
    int nsize = g->num_vertices + numVertices;

    ListPtr* var = (ListPtr*)realloc(*g->adjacency_list, nsize * sizeof(ListPtr));
    if(var == NULL) {
        return false;
    }
    else {
        //*g->adjacency_list = (ListPtr*)realloc(g->adjacency_list, nsize * sizeof(ListPtr));
        g->num_vertices = nsize;
        *g->adjacency_list = var;
        for(unsigned int i = osize; i <= nsize - 1; i++) { // create lists for every new vertices that was just added.
            g->adjacency_list[i] = list_create(VCompare, VPrinter, (void*)VFree);
        }
        g->num_vertices = nsize;
        return true;
    }
}

bool graph_remove_edge(Graph *g, int v1, int v2) {
    // removes an edge from the graph at g->adjacency_list[v1] if value is v2
    int* temp1 = (int*)malloc(sizeof(int));
    int* temp2 = (int*)malloc(sizeof(int));

    *temp1 = v1;
    *temp2 = v2;


    if((list_find_element(g->adjacency_list[*temp1], temp2) == -1) && (list_find_element(g->adjacency_list[*temp2], temp1) == -1)) { // if this specific index has no connection to vertex v2, the function call returns -1
        // if true, this means that the connection between v1 and v2 does not exist
        printf("No connection found. Remove failure\n");
        return false;
    }
    else {
        list_del_index(g->adjacency_list[*temp1], list_find_element(g->adjacency_list[*temp1], temp2));
        list_del_index(g->adjacency_list[*temp2], list_find_element(g->adjacency_list[*temp2], temp1));
        printf("Removal successful\n");
        return true;
    }
}

bool graph_has_edge(Graph *g, int v1, int v2) {
    // decides if the graph has an edge between two nodes (v1 and v2) 
    // i and j are the relevant connections in each list index
    int* temp1 = (int*)malloc(sizeof(int));
    int* temp2 = (int*)malloc(sizeof(int));
    *temp1 = v1;
    *temp2 = v2;


    if(v1 > graph_num_vertices(g) || v2 > graph_num_vertices(g) || v1 < 0 || v2 < 0) {
        return false;
    }

    int i = list_find_element(g->adjacency_list[*temp1], temp2); // v1
    int j = list_find_element(g->adjacency_list[*temp2], temp1); // v2

    if(i != -1 && j != -1) { // this confirms that v2 has a connection with v1 and vice versa
        return true;
    }
    else {
        return false;
    }
}

ListPtr graph_get_neighbors(Graph *g, int vertex) { // returns a List of every neighboring index (which pretty much is the ListPtr at the index provided and all of its nodes)
    // return the listptr at that index
    if(vertex < 0 || vertex > graph_num_vertices(g)) {
        return NULL;
    }
    if(list_length(g->adjacency_list[vertex]) > 0) {
        return g->adjacency_list[vertex]; // returns the ListPtr at that vertex which contains a list of connected vertices to that vertex
    } 
    else {
        return NULL;
    }
}

int graph_num_vertices(Graph *g) {
    return g->num_vertices;
}

int graph_num_edges(Graph *g) {
    int count = 0;

    for(unsigned int i = 0; i < g->num_vertices; i++) { // for every vertex visited, add the list length.
        count += list_length(g->adjacency_list[i]);
        
    }

    if(count % 2 == 0) { // guarantee that each edge that exists is connecting two parent vertices that share it
        return count / 2; // returns the num edges / 2 as for every edge there are two vertexes that contain that edge within their list
    }
    else {
        return -1;
    }
}