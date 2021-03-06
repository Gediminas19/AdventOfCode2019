#include <bits/stdc++.h>
using namespace std;

#define ROWCOUNT 117
#define COLCOUNT 123

struct MazeHasher {
  size_t operator()(const pair<int, int>& pos) const {
    return pos.first*COLCOUNT + pos.second;
  }
};

int main() {
  ifstream fp;
  fp.open("input.txt");

  string *raw = new string[ROWCOUNT + 4];
  for (int j = 0; j < ROWCOUNT + 4; j++) getline(fp, raw[j]);
  fp.close();

  unordered_map<string, pair<int, int>> portal_help;
  unordered_map<pair<int, int>, pair<int, int>, MazeHasher> portals;
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
          portal_help[name] = make_pair(porow, pocol);
        else {
          portals[make_pair(porow, pocol)] = portal_help[name];
          portals[portal_help[name]] = make_pair(porow, pocol);
        }
      }
    }

    if (2 <= j && j < ROWCOUNT + 2) strncpy(maze[j - 2], raw[j].c_str() + 2, COLCOUNT);
  }

  int start_row = portal_help["AA"].first;
  int start_col = portal_help["AA"].second;
  cout << "Starting at " << start_row << "," << start_col << endl;

  int target_row = portal_help["ZZ"].first;
  int target_col = portal_help["ZZ"].second;
  cout << "Aiming for " << target_row << "," << target_col << endl;

  unordered_set<pair<int, int>, MazeHasher> visited;
  queue<tuple<int, int, int>> working;
  working.push(make_tuple(start_row, start_col, 0));

  while (!working.empty()) {
    int row, col, dist;
    tie(row, col, dist) = working.front();
    working.pop();

    if (row == target_row && col == target_col) {
      cout << "Destination reached after " << dist << " steps" << endl;
      break;
    }

    if (row < 0 || col < 0 || row >= ROWCOUNT || col >= COLCOUNT || maze[row][col] != '.'
        || (visited.find(make_pair(row, col)) != visited.end())) continue;
    visited.insert(make_pair(row, col));

    // cout << "Testing at " << layer << ": " << row << "," << col << "; Dist: " << dist << endl;

    working.push(make_tuple(row + 1, col, dist + 1));
    working.push(make_tuple(row - 1, col, dist + 1));
    working.push(make_tuple(row, col + 1, dist + 1));
    working.push(make_tuple(row, col - 1, dist + 1));
    if (portals.find(make_pair(row, col)) != portals.end())
      working.push(make_tuple(portals[make_pair(row, col)].first,
                   portals[make_pair(row, col)].second, dist + 1));
  }

  return 0;
}