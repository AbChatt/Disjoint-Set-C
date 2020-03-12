/*
 * A few basic test cases are provided here for your implementations. You will
 * NOT be submitting this as part of the assignment. I strongly recommend you
 * add additional test cases here to ensure correct functionality of your code.
 *
 * Compile using `make` and run all tests using `./TestMST`. Additionally, you
 * can run individual tests by passing in the test number, like `./TestMST 3`.
 *
 * CSCB63 Winter 2020 - Assignment 3
 * (c) Mustafa Quraish
 */

#include "MST.h"

/// @brief Tests makeDisjointSets()
void test_ds0() {
  DisjointSets *ds = makeDisjointSets(10);
  assert(ds);
  assert(ds->parents);  // Make sure not NULL
  assert(ds->ranks);
  assert(ds->size == 10);
  for (int i = 0; i < 10; i++) {
    assert(ds->parents[i] == i);
    assert(ds->ranks[i] == 0);
  }
}

/// @brief Tests Path compression and find()
void test_ds1() {
  DisjointSets *ds = makeDisjointSets(10);
  // Change parents to check path compression
  for (int i = 1; i < 8; i++) {
    ds->parents[i] = i - 1;
  }
  assert(find(ds, 7) == 0);
  for (int i = 0; i < 8; i++) {
    assert(ds->parents[i] == 0);
  }
  for (int i = 8; i < 10; i++) {
    assert(ds->parents[i] == i);
  }
}

/// @brief Tests Join
void test_ds2() {
  DisjointSets *ds = makeDisjointSets(10);
  join(ds, 0, 1);  // [1, 1, 2, 3, 4, 5, 6, 7, 8, 9]
  join(ds, 2, 3);  // [1, 1, 3, 3, 4, 5, 6, 7, 8, 9]
  join(ds, 4, 5);  // [1, 1, 3, 3, 5, 5, 6, 7, 8, 9]
  join(ds, 6, 7);  // [1, 1, 3, 3, 5, 5, 7, 7, 8, 9]
  join(ds, 8, 9);  // [1, 1, 3, 3, 5, 5, 7, 7, 9, 9]
  join(ds, 0, 2);  // [1, 3, 3, 3, 5, 5, 7, 7, 9, 9]
  join(ds, 4, 6);  // [1, 3, 3, 3, 5, 7, 7, 7, 9, 9]
  join(ds, 3, 7);  // [1, 3, 3, 7, 5, 7, 7, 7, 9, 9]
  join(ds, 8, 0);  // Path compression happens here!
  int expectedParents[10] = {7, 7, 3, 7, 5, 7, 7, 7, 9, 7};
  int expectedRanks[10] = {0, 1, 0, 2, 0, 1, 0, 3, 0, 1};
  for (int i = 0; i < 10; i++) {
    assert(ds->parents[i] == expectedParents[i]);
    assert(ds->ranks[i] == expectedRanks[i]);
  }
}

/// @brief Checks if 2 Edges are equal
int compareEdge(Edge *x, Edge *y) {
  return (x->a == y->a && x->b == y->b &&
          x->cost == y->cost);  // `==` fine since doubles not changed
}

/// @brief Checks if computed MST is the same as expected one
///        Unfortunately n^2, because need to ensure any order
///        of the edges in the computed MST is fine.
///
/// Note: This is not what will be used to test your code in the
///       automarker, since this assumes that the MST is unique.
///       This might not be the case in general, and the automarker
///       will accept *any* valid solution.
void compareMSTs(Edge computed[], Edge expected[], int num_edges) {
  int found;
  for (int i = 0; i < num_edges; i++) {
    found = 0;
    for (int j = 0; j < num_edges; j++) {
      if (compareEdge(expected + i, computed + j)) {
        found = 1;
        break;
      }
    }
    assert(found);
  }
}

/// @brief Input is already a minimum spanning tree
void test_mst0() {
  Edge graph[3] = {{0, 1, 1.0}, {1, 2, 3.0}, {2, 3, 4.0}};
  Edge mst[3];
  findMST(graph, 4, 3, mst);

  Edge expected[3] = {{0, 1, 1.0}, {1, 2, 3.0}, {2, 3, 4.0}};
  compareMSTs(mst, expected, 3);
}

/// @brief Input is already a minimum spanning tree
void test_mst1() {
  Edge graph[10] = {{0, 1, 1.0},  {1, 2, 2.0}, {1, 3, 3.0}, {1, 4, 6.0},
                    {1, 5, 10.0}, {0, 4, 8.0}, {0, 3, 5.0}, {5, 3, 9.0},
                    {3, 2, 4.0},  {3, 4, 7.0}};
  Edge mst[5];
  findMST(graph, 6, 10, mst);

  Edge expected[5] = {
      {1, 2, 2.0}, {1, 3, 3.0}, {0, 1, 1.0}, {1, 4, 6.0}, {5, 3, 9.0}};
  compareMSTs(mst, expected, 3);
}

// Add more tests here to ensure your algorithm works as intended...
// Make sure you all the test case to the list below and increment
// the number of tests so that it actually runs with the others.

int NUM_TESTS = 5;
void (*TESTS[])() = {test_ds0, test_ds1, test_ds2, test_mst0, test_mst1};

int main(int argc, char *argv[]) {
  int test_num;
  if (argc > 1) {
    test_num = atoi(argv[1]);
    if (test_num >= 0 && test_num < NUM_TESTS) {
      TESTS[test_num]();  // Run the test
      printf("Test %d passed!\n", test_num);
      return 0;
    } else {
      printf("Invalid test number entered. Running all tests...\n");
    }
  }

  for (test_num = 0; test_num < NUM_TESTS; test_num++) {
    TESTS[test_num]();
    printf("Test %d passed!\n", test_num);
  }
  return 0;
}

// Fin.
