/*
 * You shouldn't change any of the code here. You will NOT be submitting this 
 * file as part of your assignment. You don't need to declare any helper 
 * functions here since they are not going to be exposed to any of the other 
 * files.
 *
 * CSCB63 Winter 2020 - Assignment 3
 * (c) Mustafa Quraish
 */

#ifndef __MST_H
#define __MST_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/// @brief Structure that stores the Disjoint Sets
typedef struct disjoint_sets {
    int *parents; /// Array containing parents of nodes
    int *ranks;   /// Array containing ranks of nodes
    int size;     /// Size of the arrays (to print)
} DisjointSets;

void printDisjointSets(DisjointSets *ds); // Provided for you!

DisjointSets *makeDisjointSets(int num_nodes);
void join(DisjointSets *ds, int x, int y);
int find(DisjointSets *ds, int x);
void freeDisjointSets(DisjointSets *ds);

/// @brief Stored the associated data for a single edge
typedef struct edge {
    int a;  /// Corresponds to node of one end
    int b;  /// ... and the other end
    double cost; /// Cost of this edge
} Edge;

int compareEdgeCosts(const void *a, const void *b);
void findMST(Edge graph[], int n, int m, Edge mst[]);

#endif
