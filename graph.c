#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

typedef struct edgeNode {
  struct edgeNode *next;
  int vertex;
  elemE weight;
} EdgeNode;

typedef struct vertexNode {
  EdgeNode *beg, *end;
  elemV data;
} VertexNode;

struct graph {
  VertexNode *list;
  int numVt;
  int numEd;
  int isDigraph;
};

// n = number of vertices, d = is digraph
Graph newGraph(int n, int d) {
  Graph g = malloc(sizeof(GRAPH));  // creates the graph
  if (g == NULL) return NULL; // error

  if (n <= 0) return NULL;  // error: number of vertices should be a natural number
  g->list = malloc(n*sizeof(VertexNode)); // instantializes the list (no edges included)
  if (g->list == NULL) return NULL; // error

  for (int i = 0; i < n; i++) {   // initializes all the begs and ends with NULL
    g->list[i].beg = NULL;
    g->list[i].end = NULL;
    g->list[i].data = 0;
  }

  g->numVt = n;
  g->numEd = 0;
  if (d < 0 || d > 1) d = 1;
  g->isDigraph = d;

  return g;
}

int adjacent(Graph g, int x, int y) {
  if (g == NULL || g->list == NULL) return -1;  // error
  if (x < 1 || x > g->numVt || y < 1 || y > g->numVt) return -1; // error

  if (g->list[x-1].beg != NULL) {
    EdgeNode *aux = g->list[x-1].beg;
    while (aux != NULL) {
      if (aux->vertex == y) return 1;
      aux = aux->next;
    }
  }
  else return 0;
}

// creates x -> y
int addEdge(Graph g, int x, int y) {
  if (g == NULL || g->list == NULL) return 0;
  if (x < 1 || x > g->numVt || y < 1 || y > g->numVt) return 0;
  if (adjacent(g, x, y)) return 0;

  EdgeNode *n = malloc(sizeof(EdgeNode)); // new edge
  if (n == NULL) return 0;
  n->next = NULL;
  n->vertex = y;
  n->weight = 1;

  if (g->list[x-1].beg == NULL) {
    g->list[x-1].beg = n;
    g->list[x-1].end = n;
  } else {
    g->list[x-1].end->next = n;
    g->list[x-1].end = n;
  }

  if (!g->isDigraph && !adjacent(g, y, x)) addEdge(g, y, x);
  g->numEd++;

  return 1;
}

// removes x -> y
int removeEdge(Graph g, int x, int y) {
  if (g == NULL || g->list == NULL) return 0;
  if (x < 1 || x > g->numVt || y < 1 || y > g->numVt) return 0;

  if (g->list[x-1].beg != NULL) {
    // tries to find y node
    EdgeNode *cur = g->list[x-1].beg;
    EdgeNode *prev = NULL;

    while (cur != NULL && cur->vertex != y) {
      prev = cur;
      cur = cur->next;
    }

    if (cur == NULL) return 0;  // there's no edge from x to y
    else {  // the edge exists!
      if (prev == NULL) {   // cur points to the beginning of the list
        g->list[x-1].beg = cur->next;  // updates value of begin
        if (cur->next == NULL) g->list[x-1].end = NULL;  // updates value of end
        free(cur);  // deletes node
      }
      else {   // cur is not on the beginning of the list
        if (cur == g->list[x-1].end) {  // y is in the last node of the list
          prev->next = NULL;  // cuts reference to cur
          g->list[x-1].end = prev;  // updates value of end
          free(cur);  // deletes node
        }
        else {  // y is in the "middle" of the list
          prev->next = cur->next;   // cuts reference to cur
          free(cur);  // deletes node
        }
      }
    }
  }
  else return 0;  // there's no edge from x to y

  if (!g->isDigraph && adjacent(g, y, x)) removeEdge(g, y, x);
  g->numEd--;

  return 1;
}

int addVertex(Graph g, int x) {     // adds the vertex x, if it is not there
  if (g == NULL || g->list == NULL) return 0;
  if (x < 1) return 0;  // number of vertex shall be at least one

  if (x > g->numVt) {
    g->list = realloc(g->list, 2*(g->numVt)*sizeof(VertexNode));
    if (g->list == NULL) return -1;   // super error: you lost the graph you had previously (probably will never happen)
    g->list[x-1].data = 0;
    g->list[x-1].beg = NULL;
    g->list[x-1].end = NULL;
    g->numVt++;
    return 1;   // success
  }
  else {
    if (g->list[x-1].beg != NULL || g->list[x-1].data != 0) return 0;
    else return 1;
  }
}

void printGraph(Graph g, int verbose) {
  if (g == NULL || g->list == NULL) return;

  EdgeNode *aux;
  for (int i = 0; i < g->numVt; i++) {
    if (g->list[i].beg != NULL) {
      aux = g->list[i].beg;
      if (verbose) printf("Vertex %d (%c) -> ", i+1, g->list[i].data);
      else printf("%d -> ", i+1);
      while (aux->next != NULL) {
        if (verbose) printf("(%d, %.2lf), ", aux->vertex, aux->weight);
        else printf("%d, ", aux->vertex);
        aux = aux->next;
      }
      if (verbose) printf("(%d, %.2lf)\n", aux->vertex, aux->weight);
      else printf("%d\n", aux->vertex);
    }
    else {
      if (verbose) printf("Vertex %d (%c) -> NONE\n", i+1, g->list[i].data);
      else printf("%d -> NONE\n", i+1);
    }
  }
}

void deleteGraph(Graph g) {
  if (g == NULL) return;

  // free list:
  EdgeNode *aux;
  if (g->list != NULL) {
    for (int i = 0; i < g->numVt; i++) {
      if (g->list[i].beg != NULL) {
        aux = g->list[i].beg;
        while (g->list[i].beg->next != NULL) {
          g->list[i].beg = g->list[i].beg->next;
          free(aux);
          aux = g->list[i].beg;
        }
        free(aux);
        //g->list[i].beg = NULL;
        //g->list[i].end = NULL;
      }
    }
    free(g->list);
  }

  // free graph:
  free(g);
}
