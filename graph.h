#ifndef GRAPH_H
#define GRAPH_H

typedef double elemE;
typedef char elemV;
typedef struct graph GRAPH;
typedef GRAPH* Graph;

/*
  By default, in functions that returns an integer as error flag, 0 stands for error and 1 for success.
  The same happens with functions that return pointers: NULL represents error.
*/

Graph newGraph(int n, int d); // returns a new instantialized graph. n = number of vertices to be created, d = is digraph (1), or not (0).
int adjacent(Graph g, int x, int y);  // tests whether there is an edge from the vertex x to the vertex y.  Returned int is like a boolean. If -1 is returned, then an error occured.
int addEdge(Graph g, int x, int y);  // adds an edge from vertex x to vertex y.
int removeEdge(Graph g, int x, int y); // removes the edge from vertex x to vertex y.
int addVertex(Graph g, int x);  // adds the vertex x, if it is not there.
int removeVertex(Graph g, int x);  // removes the vertex x, if it is there.
void printGraph(Graph g, int verbose); // if an error occurs, the function doesn't print anything.
void deleteGraph(Graph g); // tries to destroy graph g

/*
  TODO:
    neighbors(G, x): lists all vertices y such that there is an edge from the vertex x to the vertex y;
    get_edge_value(G, x, y): returns the value associated with the edge (x, y);
    set_edge_value(G, x, y, v): sets the value associated with the edge (x, y) to v.
    get_vertex_value(G, x): returns the value associated with the vertex x;
    set_vertex_value(G, x, v): sets the value associated with the vertex x to v.
*/

#endif
