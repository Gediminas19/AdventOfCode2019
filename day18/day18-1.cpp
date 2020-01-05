#include <bits/stdc++.h>
using namespace std;

#define ROWCOUNT 81
#define COLCOUNT 81
// #define ROWCOUNT 9
// #define COLCOUNT 17

struct RoomHasher {
  size_t operator()(const pair<int, int>& pos) const {
    return pos.first*COLCOUNT + pos.second;
  }
};

struct MemoHasher {
  size_t operator()(const pair<pair<int, int>, set<char>>& memodata) const {
    size_t h = 0;
    for (char key_left : memodata.second) h = h*31 + key_left;
    h = h*31 + memodata.first.first*COLCOUNT + memodata.first.second;
    return h;
  }
};

struct DistCompare {
  bool operator()(const tuple<int, int, int>& dest1, const tuple<int, int, int>& dest2) const {
    int row1, col1, dist1, row2, col2, dist2;
    tie(row1, col1, dist1) = dest1;
    tie(row2, col2, dist2) = dest2;
    return dist1 > dist2;
  }
};

int recurse(char room[ROWCOUNT][COLCOUNT], unordered_map<pair<int, int>, int, RoomHasher> neighbors[ROWCOUNT][COLCOUNT],
             unordered_map<char, pair<int, int>> doors, unordered_map<pair<pair<int, int>, set<char>>, int, MemoHasher>& memoizer,
             set<char>& keysleft, int start_row, int start_col) {

  if (keysleft.empty()) {
    // cout << "When starting from " << start_row << "," << start_col << " and the keys left are: ";
    // for (auto key : keysleft) cout << key;
    // cout << endl << "the dist needed is 0" << endl;
    memoizer[make_pair(make_pair(start_row, start_col), keysleft)] = 0;
    return 0;
  }

  auto lookup = memoizer.find(make_pair(make_pair(start_row, start_col), keysleft));
  if (lookup != memoizer.end()) return lookup->second;

  int dists[ROWCOUNT][COLCOUNT];
  bool visited[ROWCOUNT][COLCOUNT];
  for (int j = 0; j < ROWCOUNT; j++) {
    for (int k = 0; k < COLCOUNT; k++) {
      dists[j][k] = INT_MAX;
      visited[j][k] = false;
    }
  }

  priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, DistCompare> working;
  priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, DistCompare> reachable_keys;

  dists[start_row][start_col] = 0;
  working.push(make_tuple(start_row, start_col, dists[start_row][start_col]));

  while (!working.empty()) {
    int row, col, dist;
    tie(row, col, dist) = working.top();
    working.pop();

    // if visited or is locked door, skip
    if (visited[row][col] || isupper(room[row][col])) continue;
    visited[row][col] = true;

    // cout << "Examining " << room[row][col] << " at " << row << "," << col << " with dist " << dist << endl;

    if (islower(room[row][col])) {
      reachable_keys.push(make_tuple(row, col, dist));
      continue;
    }

    for (auto neighbor: neighbors[row][col]) {
      int nrow = neighbor.first.first, ncol = neighbor.first.second, ndist = neighbor.second;
      // cout << nrow << "," << ncol << " at dist " << dist + ndist << endl;
      if (!visited[nrow][ncol] && dist + ndist < dists[nrow][ncol]) {
        dists[nrow][ncol] = dist + ndist;
        working.push(make_tuple(nrow, ncol, dists[nrow][ncol]));
      }
    }
  }

  int potdist = INT_MAX;
  room[start_row][start_col] = '.';
  // otherwise, try out the possible keys
  while (!reachable_keys.empty()) {
    int row, col, dist;
    tie(row, col, dist) = reachable_keys.top();
    reachable_keys.pop();

    char key = room[row][col];
    bool for_door = (doors.find(key) != doors.end());
    // cout << "Take key " << key << " which is " << dist << " away" << endl;

    room[row][col] = '@';
    if (for_door) room[doors[key].first][doors[key].second] = '.';
    keysleft.erase(key);

    int dist_left = recurse(room, neighbors, doors, memoizer, keysleft, row, col);
    if (dist_left + dist < potdist) potdist = dist_left + dist;

    keysleft.insert(key);
    room[row][col] = key;
    if (for_door) room[doors[key].first][doors[key].second] = toupper(key);
  }
  room[start_row][start_col] = '@';

  memoizer[make_pair(make_pair(start_row, start_col), keysleft)] = potdist;
  // cout << "When starting from " << start_row << "," << start_col << " and the keys left are: ";
  // for (auto key : keysleft) cout << key;
  // cout << endl << "the dist needed is " << potdist << endl;
  return potdist;
}

int main() {
  ifstream fp;
  fp.open("input.txt");

  string *raw = new string[ROWCOUNT];
  for (int j = 0; j < ROWCOUNT; j++) getline(fp, raw[j]);
  fp.close();

  char room[ROWCOUNT][COLCOUNT];
  for (int j = 0; j < ROWCOUNT; j++) {
    strncpy(room[j], raw[j].c_str(), COLCOUNT);
  }

  int start_row, start_col;
  unordered_map<pair<int, int>, int, RoomHasher> neighbors[ROWCOUNT][COLCOUNT];
  unordered_map<char, pair<int, int>> alphalocs;
  for (int j = 0; j < ROWCOUNT; j++) {
    for (int k = 0; k < COLCOUNT; k++) {
      if (room[j][k] == '@') {
        start_row = j;
        start_col = k;
      }
      if (isalpha(room[j][k])) {
        alphalocs[room[j][k]] = make_pair(j, k);
      }
      if (room[j][k] != '#') {
        if (room[j + 1][k] != '#') neighbors[j][k][make_pair(j + 1, k)] = 1;
        if (room[j - 1][k] != '#') neighbors[j][k][make_pair(j - 1, k)] = 1;
        if (room[j][k + 1] != '#') neighbors[j][k][make_pair(j, k + 1)] = 1;
        if (room[j][k - 1] != '#') neighbors[j][k][make_pair(j, k - 1)] = 1;
      }
    }
  }

  unordered_map<char, pair<int, int>> doors;
  for (auto elem : alphalocs) {
    if (islower(elem.first) && alphalocs.find(toupper(elem.first)) != alphalocs.end()) {
      doors[elem.first] = alphalocs[toupper(elem.first)];
    }
  }

  bool reducible = true;
  while (reducible) {
    reducible = false;
    for (int j = 0; j < ROWCOUNT; j++) {
      for (int k = 0; k < COLCOUNT; k++) {
        if (room[j][k] == '.') {
          if (neighbors[j][k].size() == 2) {
            reducible = true;

            auto neighbor = neighbors[j][k].begin();
            int row1 = neighbor->first.first, col1 = neighbor->first.second, dist1 = neighbor->second;
            neighbor++;
            int row2 = neighbor->first.first, col2 = neighbor->first.second, dist2 = neighbor->second;

            neighbors[row1][col1].erase(make_pair(j, k));
            neighbors[row2][col2].erase(make_pair(j, k));
            neighbors[row1][col1][make_pair(row2, col2)] = dist1 + dist2;
            neighbors[row2][col2][make_pair(row1, col1)] = dist1 + dist2;
            neighbors[j][k].clear();
          }

          if (neighbors[j][k].size() == 1) {
            reducible = true;

            auto neighbor = neighbors[j][k].begin();
            int row = neighbor->first.first, col = neighbor->first.second, dist = neighbor->second;

            neighbors[row][col].erase(make_pair(j, k));
            neighbors[j][k].clear();
          }
        }
      }
    }
  }

  unordered_map<pair<pair<int, int>, set<char>>, int, MemoHasher> memoizer;
  set<char> keysleft;
  for (auto elem : alphalocs) if (islower(elem.first)) keysleft.insert(elem.first);

  int mindist = INT_MAX;
  recurse(room, neighbors, doors, memoizer, keysleft, start_row, start_col);
  // cout << "Min dist to all keys is " << mindist << endl;

  cout << "Total steps needed: " << memoizer[make_pair(make_pair(start_row, start_col), keysleft)] << endl;

  return 0;
}