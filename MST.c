/*
 * This is the file where all of your implementation goes. You will be
 * submitting (only) this file to MarkUs.
 *
 * TestMST.c provides some test cases to test both your implementation for
 * disjoint sets as well as Kruskal's algorithm. You can compile it using
 * the Makefile and running the executable produced.
 *
 * As always, note that passing all the test cases does not mean your solution
 * is fully functional, that is left for you to test yourself. They are simply
 * there to let you know you are on the right track.
 *
 * CSCB63 Winter 2020 - Assignment 3
 * (c) Mustafa Quraish
 */

#include "MST.h"

/// @brief Prints out the data corresponding to the disjoint sets structure.
///        Only here for debugging. Feel free to change this if you'd like.

void printDisjointSets(DisjointSets *ds) {
  printf("Parents: [");
  for (int i = 0; i < ds->size; i++) printf("%d, ", ds->parents[i]);
  printf("]\n");
  printf("Ranks  : [");
  for (int i = 0; i < ds->size; i++) printf("%d, ", ds->ranks[i]);
  printf("]\n");
}

/*****************************************************************************/

/// @brief Allocates and initializes a Disjoint Sets structure for `n`
///        different nodes. For simplicity, can assume all nodes are going to
///        be numbered [0 : n-1]. Make sure you use dynamic memory allocation,
///        and exit the program if an error occurs.
///
/// @param n  Number of nodes
///
/// @TODO: Finish this function. Make sure the `parents` and `ranks` arrays are
///        allocated and initialized appropriately. You are effectively calling
///        MakeSet() on all the nodes

DisjointSets *makeDisjointSets(int n) {
  DisjointSets *new_disjoint_set = malloc(1 * sizeof(DisjointSets));

  if (new_disjoint_set == NULL) {
    printf("Error occurred! Exiting program...\n");
    return new_disjoint_set;
  }

  new_disjoint_set->parents = malloc(n * sizeof(int *));

  if (new_disjoint_set->parents == NULL) {
    printf("Error occurred! Exiting program...\n");
    return NULL;
  }

  new_disjoint_set->ranks = malloc(n * sizeof(int *));

  if (new_disjoint_set->ranks == NULL) {
    printf("Error occurred! Exiting program...\n");
    return NULL;
  }

  new_disjoint_set->size = 0;

  for (int i = 0; i < n; i++) {
    new_disjoint_set->parents[i] = i;
    new_disjoint_set->ranks[i] = 0;
    new_disjoint_set->size++;    
  }
  
  return new_disjoint_set;
}

/// @brief Finds the root of a node in the given Disjoint Sets.
///
/// @param ds   Disjoint Sets structure
/// @param x    Node for which the root is to be found
///
/// @TODO: Find and return the root of the node `x`. Make sure you perform path
///        compression while you are performing the find.

int find(DisjointSets *ds, int n) {  // with compress
  int root = -1;
  int parent = -1;
  int temp = -1;

  root = n;

  while (ds->parents[root] != root) {
    root = ds->parents[root];
  }

  parent = n;

  // path compression
  while (ds->parents[parent] != root) {
    temp = ds->parents[parent];
    ds->parents[parent] = root;
    parent = temp;
  }
  
  return root;
}

/// @brief Permorms the union of two nodes in the Disjoint Sets
///
/// @param ds   Disjoint Sets structure
/// @param x,y  Nodes to be unioned together
///
/// @TODO: Perform the union of the two nodes using the algorithm you have
///        learned. Specifically, perform Union by rank. If the roots of the
///        nodes have the same rank, then set the parent of x's root to y's
///        root, and increase the latter's rank by one. Otherwise, the root
///        with the higher rank becomes the parent of the other.

void join(DisjointSets *ds, int x, int y) {
  int root_x = -1;
  int root_y = -1;

  root_x = find(ds, x);
  root_y = find(ds, y);

  if (ds->ranks[root_x] > ds->ranks[root_y]) {
    ds->parents[root_y] = root_x;
  }
  else if (ds->ranks[root_x] < ds->ranks[root_y]) {
    ds->parents[root_x] = root_y;
  }
  else
  {
    ds->parents[root_x] = root_y;
    ds->ranks[root_y]++;
  }
}

/// @brief Free up the memory allocated into the Disjoint Sets structure
///
/// @param ds   Disjoint Sets structure
///
/// @TODO: Free all the memory allocated, including the arrays inside as well
///        as the actual struct itself.

void freeDisjointSets(DisjointSets *ds) {
  
  free(ds->parents);
  free(ds->ranks);
  free(ds);
}

/// @brief Comparison function (used in qsort) to help sort the edges.
///
/// @param x, y   These are effectively going to be `const Edge *`. They are
///               only `const void *` since that is the type required by the
///               qsort() function. You can cast them to Edge pointers to
///               actually extract the value as follows:
///                       const Edge *a = (const Edge *) x;
///
/// @TODO: If the cost for the edge `x` is greater than (or equal) to the cost
///        of edge `y`, return 1. Otherwise, return 0. (This function should
///        not really take you more than 3 lines - it's just there because you
///        need it)

int compareEdgeCosts(const void *x, const void *y) {
  const Edge *a = (const Edge *)x;
  const Edge *b = (const Edge *)y;

  if (a->cost >= b->cost) {
    return 1;
  }

  return 0;
}

/// @brief Performs Kruskal's algorithm on the given graph and returns the
///        Minimum spanning tree found as a result.
///
/// @param n      The number of nodes in the graph. These nodes don't actually
///               store any information, so we don't store them anywhere. The
///               nodes will be labelled [0 : n-1].
/// @param m      The number of edges in the graph
/// @param graph  A list of `m` weighted edges. Each edge stores both it's end
///               points as well as it's cost
///
/// @param mst    An empty list with enough space for `n-1` edges. Store all
///               the edges that are part of the MST in here, in any order.
///               Note that any MST on a graph of order n always has n-1 nodes.
///
/// @TODO: Complete this function. You may want to look into how the function
///        `qsort()` works in case you need it (hint hint).

void findMST(Edge graph[], int n, int m, Edge mst[]) {
  DisjointSets *ds = makeDisjointSets(n);

  qsort(graph, m, sizeof(Edge), compareEdgeCosts);

  for (int i = 0; i < n; i++) {
    if (ds->parents[graph[i].a] != ds->parents[graph[i].b]) {
      join(ds, graph[i].a, graph[i].b);
      mst[i] = graph[i];
    }
  }
}
