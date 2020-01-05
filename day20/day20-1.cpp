#include <bits/stdc++.h>
using namespace std;


int main() {
  ifstream fp;
  fp.open("input.txt");

  int rowcount = 117, colcount = 123;
  // int rowcount = 33, colcount = 31;

  string *raw = new string[rowcount + 4];
  for (int j = 0; j < rowcount + 4; j++) getline(fp, raw[j]);
  fp.close();

  unordered_map<string, int> portal_help;
  unordered_map<int, int> portals;
  char maze[rowcount][colcount];
  for (int j = 0; j < rowcount + 4; j++) {
    for (int k = 0; k < raw[j].length(); k++) {
      if (isalpha(raw[j][k])) {
        string name;
        int porow, pocol;

        if (isalpha(raw[j + 1][k])) {
          name = raw[j].substr(k, 1) + raw[j + 1].substr(k, 1);
          raw[j][k] = ' ';
          raw[j + 1][k] = ' ';

          pocol = k - 2;
          if (j < 2 || (j > rowcount/2 + 2 && j < rowcount + 2)) porow = j;
          else porow = j - 3;
        } else {
          name = raw[j].substr(k, 2);
          raw[j][k] = ' ';
          raw[j][k + 1] = ' ';

          porow = j - 2;
          if (k < 2 || (k > colcount/2 + 2 && k < colcount + 2)) pocol = k;
          else pocol = k - 3;
        }
        if (portal_help.find(name) == portal_help.end()) portal_help[name] = porow*colcount + pocol;
        else {
          portals[porow*colcount + pocol] = portal_help[name];
          portals[portal_help[name]] = porow*colcount + pocol;
        }
      }
    }

    if (2 <= j && j < rowcount + 2) strncpy(maze[j - 2], raw[j].c_str() + 2, colcount);
  }

  cout << "There are " << (portals.size()/2) << " pairs of portals" << endl;

  int start_row = portal_help["AA"]/colcount;
  int start_col = portal_help["AA"]%colcount;
  cout << "Starting at " << start_row << "," << start_col << endl;

  int target_row = portal_help["ZZ"]/colcount;
  int target_col = portal_help["ZZ"]%colcount;
  cout << "Aiming for " << target_row << "," << target_col << endl;

  queue<pair<int, int>> working;
  working.push(make_pair(portal_help["AA"], 0));

  while (!working.empty()) {
    int row = working.front().first/colcount;
    int col = working.front().first%colcount;
    int dist = working.front().second;
    working.pop();

    // cout << "Testing at " << row << "," << col << endl;

    if (row == target_row && col == target_col) {
      cout << "Destination reached after " << dist << " steps" << endl;
      break;
    }

    if (row < 0 || col < 0 || row >= rowcount || col >= colcount
        || maze[row][col] == '#' || maze[row][col] == ' ') continue;
    maze[row][col] = ' ';

    working.push(make_pair((row + 1)*colcount + col, dist + 1));
    working.push(make_pair((row - 1)*colcount + col, dist + 1));
    working.push(make_pair(row*colcount + (col + 1), dist + 1));
    working.push(make_pair(row*colcount + (col - 1), dist + 1));
    if (portals.find(row*colcount + col) != portals.end())
      working.push(make_pair(portals[row*colcount + col], dist + 1));
  }

  // for (int j = 0; j < rowcount; j++) {
  //   for (int k = 0; k < colcount; k++) cout << maze[j][k];
  //   cout << endl;
  // }

  return 0;
}