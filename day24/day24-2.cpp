#include <bits/stdc++.h>
using namespace std;

struct BugHasher {
  size_t operator()(const tuple<int, int, int>& bug) const {
    int layer, row, col;
    tie(layer, row, col) = bug;
    return layer*25 + row*5 + col;
  }
};

int main() {
  ifstream fp;
  fp.open("input.txt");

  int iters = 200;
  unordered_set<tuple<int, int, int>, BugHasher> bugs[iters + 1];

  int dim = 5;
  string read;
  for (int j = 0; j < dim; j++) {
    getline(fp, read);
    for (int k = 0; k < dim; k++)
      if (read[k] == '#') bugs[0].insert(make_tuple(0, j, k));
  }
  fp.close();

  for (int i = 0; i < iters; i++) {
    // cout << "After " << i << " minutes, there are " << bugs[i].size() << " bugs" << endl;

    // for each currently alive bug, we increment the neighbor_count of
    // each of its neighbors by 1; due to the recursive nature
    // of our grids, neighbors are not as straightforward as part 1
    unordered_map<tuple<int, int, int>, int, BugHasher> neighbor_count;
    for (tuple<int, int, int> bug : bugs[i]) {
      int layer, row, col;
      tie(layer, row, col) = bug;

      switch (row) {
        case 0:
          neighbor_count[make_tuple(layer - 1, 1, 2)] += 1;
          neighbor_count[make_tuple(layer, 1, col)] += 1;
          break;
        case 1:
          neighbor_count[make_tuple(layer, 0, col)] += 1;
          if (col != 2) neighbor_count[make_tuple(layer, 2, col)] += 1;
          else for (int j = 0; j < 5; j++) neighbor_count[make_tuple(layer + 1, 0, j)] += 1;
          break;
        case 2:
          neighbor_count[make_tuple(layer, 1, col)] += 1;
          neighbor_count[make_tuple(layer, 3, col)] += 1;
          break;
        case 3:
          neighbor_count[make_tuple(layer, 4, col)] += 1;
          if (col != 2) neighbor_count[make_tuple(layer, 2, col)] += 1;
          else for (int j = 0; j < 5; j++) neighbor_count[make_tuple(layer + 1, 4, j)] += 1;
          break;
        case 4:
          neighbor_count[make_tuple(layer - 1, 3, 2)] += 1;
          neighbor_count[make_tuple(layer, 3, col)] += 1;
          break;
      }
      switch (col) {
        case 0:
          neighbor_count[make_tuple(layer - 1, 2, 1)] += 1;
          neighbor_count[make_tuple(layer, row, 1)] += 1;
          break;
        case 1:
          neighbor_count[make_tuple(layer, row, 0)] += 1;
          if (row != 2) neighbor_count[make_tuple(layer, row, 2)] += 1;
          else for (int j = 0; j < 5; j++) neighbor_count[make_tuple(layer + 1, j, 0)] += 1;
          break;
        case 2:
          neighbor_count[make_tuple(layer, row, 1)] += 1;
          neighbor_count[make_tuple(layer, row, 3)] += 1;
          break;
        case 3:
          neighbor_count[make_tuple(layer, row, 4)] += 1;
          if (row != 2) neighbor_count[make_tuple(layer, row, 2)] += 1;
          else for (int j = 0; j < 5; j++) neighbor_count[make_tuple(layer + 1, j, 4)] += 1;
          break;
        case 4:
          neighbor_count[make_tuple(layer - 1, 2, 3)] += 1;
          neighbor_count[make_tuple(layer, row, 3)] += 1;
          break;
      }
    }

    // neighbor count contains cells with at least 1 neighboring bug
    // from these cells, those that are already bugs must only have 1 neighbor to remain bug
    // and those that are empty must have 1 or 2 neighbors to turn bug
    // note that if a bug isn't even in neighbor_count it must have 0 neighbors and will die
    for (pair<tuple<int, int, int>, int> pot_bug_info : neighbor_count) {
      if (bugs[i].find(pot_bug_info.first) != bugs[i].end()) {
        if (pot_bug_info.second == 1) bugs[i + 1].insert(pot_bug_info.first);
      } else {
        if (pot_bug_info.second == 1 || pot_bug_info.second == 2) bugs[i + 1].insert(pot_bug_info.first);
      }
    }
  }
  cout << "After " << iters << " minutes, there are " << bugs[iters].size() << " bugs" << endl;

  return 0;
}