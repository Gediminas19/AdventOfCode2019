#include <bits/stdc++.h>
using namespace std;

#define ROWCOUNT 81
#define COLCOUNT 81
// #define ROWCOUNT 9
// #define COLCOUNT 17

struct MemoHasher {
  size_t operator()(const pair<int, set<char>>& memodata) const {
    size_t h = 0;
    for (char key_left : memodata.second) h = h*31 + key_left;
    return h*31 + memodata.first;
  }
};

struct DistCompare {
  bool operator()(const pair<int, int>& dest1, const pair<int, int>& dest2) const {
    return dest1.second > dest2.second;
  }
};

int recurse(char *room, unordered_map<int, int> *neighbors, unordered_map<char, int> doors,
            unordered_map<pair<int, set<char>>, int, MemoHasher>& memoizer, set<char>& keysleft,
            int start_pos) {

  if (keysleft.empty()) {
    // cout << "When starting from " << start_row << "," << start_col << " and the keys left are: ";
    // for (auto key : keysleft) cout << key;
    // cout << endl << "the dist needed is 0" << endl;
    memoizer[make_pair(start_pos, keysleft)] = 0;
    return 0;
  }

  auto lookup = memoizer.find(make_pair(start_pos, keysleft));
  if (lookup != memoizer.end()) return lookup->second;

  int dists[ROWCOUNT*COLCOUNT];
  bool visited[ROWCOUNT*COLCOUNT];
  for (int j = 0; j < ROWCOUNT*COLCOUNT; j++) {
    dists[j] = INT_MAX;
    visited[j] = false;
  }

  priority_queue<pair<int, int>, vector<pair<int, int>>, DistCompare> working;
  priority_queue<pair<int, int>, vector<pair<int, int>>, DistCompare> reachable_keys;

  dists[start_pos] = 0;
  working.push(make_pair(start_pos, dists[start_pos]));

  while (!working.empty()) {
    int pos = working.top().first, dist = working.top().second;
    working.pop();

    // if visited or is locked door, skip
    if (visited[pos] || isupper(room[pos])) continue;
    visited[pos] = true;

    // cout << "Examining " << room[pos] << " at " << row << "," << col << " with dist " << dist << endl;

    if (islower(room[pos])) {
      reachable_keys.push(make_pair(pos, dist));
      continue;
    }

    for (auto neighbor: neighbors[pos]) {
      int npos = neighbor.first, ndist = neighbor.second;
      // cout << nrow << "," << ncol << " at dist " << dist + ndist << endl;
      if (!visited[npos] && dist + ndist < dists[npos]) {
        dists[npos] = dist + ndist;
        working.push(make_pair(npos, dists[npos]));
      }
    }
  }

  int potdist = INT_MAX;
  room[start_pos] = '.';
  // otherwise, try out the possible keys
  while (!reachable_keys.empty()) {
    int pos = reachable_keys.top().first, dist = reachable_keys.top().second;
    reachable_keys.pop();

    char key = room[pos];
    bool for_door = (doors.find(key) != doors.end());
    // cout << "Take key " << key << " which is " << dist << " away" << endl;

    room[pos] = '@';
    if (for_door) room[doors[key]] = '.';
    keysleft.erase(key);

    int dist_left = recurse(room, neighbors, doors, memoizer, keysleft, pos);
    if (dist_left + dist < potdist) potdist = dist_left + dist;

    keysleft.insert(key);
    room[pos] = key;
    if (for_door) room[doors[key]] = toupper(key);
  }
  room[start_pos] = '@';

  memoizer[make_pair(start_pos, keysleft)] = potdist;
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

  char room[ROWCOUNT*COLCOUNT];
  for (int j = 0; j < ROWCOUNT; j++) {
    strncpy(room + j*COLCOUNT, raw[j].c_str(), COLCOUNT);
  }

  int start_pos;
  unordered_map<int, int> neighbors[ROWCOUNT*COLCOUNT];
  unordered_map<char, int> alphalocs;
  for (int j = 0; j < ROWCOUNT*COLCOUNT; j++) {
    if (room[j] == '@') start_pos = j;
    if (isalpha(room[j])) alphalocs[room[j]] = j;
    if (room[j] != '#') {
      if (room[j + 1] != '#') neighbors[j][j + 1] = 1;
      if (room[j - 1] != '#') neighbors[j][j - 1] = 1;
      if (room[j + COLCOUNT] != '#') neighbors[j][j + COLCOUNT] = 1;
      if (room[j - COLCOUNT] != '#') neighbors[j][j - COLCOUNT] = 1;
    }
  }

  unordered_map<char, int> doors;
  for (auto elem : alphalocs) {
    if (islower(elem.first) && alphalocs.find(toupper(elem.first)) != alphalocs.end()) {
      doors[elem.first] = alphalocs[toupper(elem.first)];
    }
  }

  bool reducible = true;
  while (reducible) {
    reducible = false;
    for (int j = 0; j < ROWCOUNT*COLCOUNT; j++) {
      if (room[j] == '.') {
        if (neighbors[j].size() == 2) {
          reducible = true;

          auto neighbor = neighbors[j].begin();
          int pos1 = neighbor->first, dist1 = neighbor->second;
          neighbor++;
          int pos2 = neighbor->first, dist2 = neighbor->second;

          neighbors[pos1].erase(j);
          neighbors[pos2].erase(j);
          neighbors[pos1][pos2] = dist1 + dist2;
          neighbors[pos2][pos1] = dist1 + dist2;
          neighbors[j].clear();
        }

        if (neighbors[j].size() == 1) {
          reducible = true;

          auto neighbor = neighbors[j].begin();
          int pos = neighbor->first, dist = neighbor->second;

          neighbors[pos].erase(j);
          neighbors[j].clear();
        }
      }
    }
  }

  unordered_map<pair<int, set<char>>, int, MemoHasher> memoizer;
  set<char> keysleft;
  for (auto elem : alphalocs) if (islower(elem.first)) keysleft.insert(elem.first);

  int mindist = INT_MAX;
  recurse(room, neighbors, doors, memoizer, keysleft, start_pos);
  // cout << "Min dist to all keys is " << mindist << endl;

  cout << "Total steps needed: " << memoizer[make_pair(start_pos, keysleft)] << endl;

  return 0;
}