#include <bits/stdc++.h>
using namespace std;

// #define ROWCOUNT 33
// #define COLCOUNT 41
#define ROWCOUNT 117
#define COLCOUNT 123

struct PosHasher {
  size_t operator()(const pair<int, int>& pos) const {
    return pos.second + pos.first*ROWCOUNT*COLCOUNT;
  }
};

int main() {
  ifstream fp;
  fp.open("input.txt");

  string *raw = new string[ROWCOUNT + 4];
  for (int j = 0; j < ROWCOUNT + 4; j++) getline(fp, raw[j]);
  fp.close();

  unordered_map<string, int> portal_help;
  unordered_map<int, pair<int, bool>> portals;
  char maze[ROWCOUNT][COLCOUNT];
  for (int j = 0; j < ROWCOUNT + 4; j++) {
    for (int k = 0; k < raw[j].length(); k++) {
      if (isalpha(raw[j][k])) {
        string name;
        int porow, pocol;
        bool deeper;

        if (isalpha(raw[j + 1][k])) {
          name = raw[j].substr(k, 1) + raw[j + 1].substr(k, 1);
          raw[j][k] = ' ';
          raw[j + 1][k] = ' ';

          pocol = k - 2;
          if (j < 2 || (j > ROWCOUNT/2 + 2 && j < ROWCOUNT + 2)) {
            porow = j;
            deeper = (j > ROWCOUNT/2 + 2);
          }
          else {
            porow = j - 3;
            deeper = (j <= ROWCOUNT/2 + 2);
          }
        } else {
          name = raw[j].substr(k, 2);
          raw[j][k] = ' ';
          raw[j][k + 1] = ' ';

          porow = j - 2;
          if (k < 2 || (k > COLCOUNT/2 + 2 && k < COLCOUNT + 2)) {
            pocol = k;
            deeper = (k > COLCOUNT/2 + 2);
          }
          else {
            pocol = k - 3;
            deeper = (k <= COLCOUNT/2 + 2);
          }
        }
        if (portal_help.find(name) == portal_help.end())
          portal_help[name] = porow*COLCOUNT + pocol;
        else {
          portals[porow*COLCOUNT + pocol] = make_pair(portal_help[name], deeper);
          portals[portal_help[name]] = make_pair(porow*COLCOUNT + pocol, !deeper);
        }
      }
    }

    if (2 <= j && j < ROWCOUNT + 2) strncpy(maze[j - 2], raw[j].c_str() + 2, COLCOUNT);
  }

  cout << "There are " << (portals.size()/2) << " pairs of portals" << endl;

  int start_row = portal_help["AA"]/COLCOUNT;
  int start_col = portal_help["AA"]%COLCOUNT;
  cout << "Starting at " << start_row << "," << start_col << endl;

  int target_row = portal_help["ZZ"]/COLCOUNT;
  int target_col = portal_help["ZZ"]%COLCOUNT;
  cout << "Aiming for " << target_row << "," << target_col << endl;

  unordered_set<pair<int, int>, PosHasher> visited;
  queue<tuple<int, int, int>> working;
  working.push(make_tuple(0, portal_help["AA"], 0));

  while (!working.empty()) {
    int layer, index, dist;
    tie(layer, index, dist) = working.front();
    working.pop();

    int row = index/COLCOUNT;
    int col = index%COLCOUNT;

    if (layer == 0 && row == target_row && col == target_col) {
      cout << "Destination reached after " << dist << " steps" << endl;
      break;
    }

    if (row < 0 || col < 0 || row >= ROWCOUNT || col >= COLCOUNT || maze[row][col] != '.'
        || (visited.find(make_pair(layer, index)) != visited.end())) continue;
    visited.insert(make_pair(layer, index));

    // cout << "Testing at " << layer << ": " << row << "," << col << "; Dist: " << dist << endl;

    working.push(make_tuple(layer, (row + 1)*COLCOUNT + col, dist + 1));
    working.push(make_tuple(layer, (row - 1)*COLCOUNT + col, dist + 1));
    working.push(make_tuple(layer, row*COLCOUNT + (col + 1), dist + 1));
    working.push(make_tuple(layer, row*COLCOUNT + (col - 1), dist + 1));
    if (portals.find(row*COLCOUNT + col) != portals.end()) {
      if (portals[row*COLCOUNT + col].second)
        working.push(make_tuple(layer + 1, portals[row*COLCOUNT + col].first, dist + 1));
      else if (layer > 0)
        working.push(make_tuple(layer - 1, portals[row*COLCOUNT + col].first, dist + 1));
    }
  }

  return 0;
}