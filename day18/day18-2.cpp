#include <bits/stdc++.h>
using namespace std;

#define ROWCOUNT 81
#define COLCOUNT 81
// #define ROWCOUNT 9
// #define COLCOUNT 13

struct MemoHasher {
  size_t operator()(const pair<array<int, 4>, set<char>>& memodata) const {
    size_t h = 0;
    for (char key_left : memodata.second) h = h*31 + key_left;
    for (int j = 0; j < 4; j++) h = h*31 + memodata.first[j];
    return h;
  }
};

struct DistCompare {
  bool operator()(const pair<int, int>& dest1, const pair<int, int>& dest2) const {
    return dest1.second > dest2.second;
  }
};

struct DistCompareQuad {
  bool operator()(const tuple<int, int, int>& dest1, const tuple<int, int, int>& dest2) const {
    return get<2>(dest1) > get<2>(dest2);
  }
};

int recurse(char *room, unordered_map<int, int> *neighbors, unordered_map<char, int> doors,
            unordered_map<pair<array<int, 4>, set<char>>, int, MemoHasher>& memoizer,
            set<char>& keysleft, array<int, 4>& start_poses) {

  if (keysleft.empty()) {
    // cout << "When starting from " << start_row << "," << start_col << " and the keys left are: ";
    // for (auto key : keysleft) cout << key;
    // cout << endl << "the dist needed is 0" << endl;
    memoizer[make_pair(start_poses, keysleft)] = 0;
    return 0;
  }

  auto lookup = memoizer.find(make_pair(start_poses, keysleft));
  if (lookup != memoizer.end()) return lookup->second;

  int dists[ROWCOUNT*COLCOUNT];
  bool visited[ROWCOUNT*COLCOUNT];
  for (int j = 0; j < ROWCOUNT*COLCOUNT; j++) {
    dists[j] = INT_MAX;
    visited[j] = false;
  }

  priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, DistCompareQuad> reachable_keys;
  for (int i = 0; i < 4; i++) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, DistCompare> working;

    int start_pos = start_poses[i];
    dists[start_pos] = 0;
    working.push(make_pair(start_pos, dists[start_pos]));

    while (!working.empty()) {
      int pos = working.top().first, dist = working.top().second;
      working.pop();

      // if visited or is locked door, skip
      if (visited[pos] || isupper(room[pos])) continue;
      visited[pos] = true;

      // cout << "Examining " << room[pos] << " at " << (pos/COLCOUNT) << "," << (pos%COLCOUNT) << " with dist " << dist << endl;

      if (islower(room[pos])) {
        reachable_keys.push(make_tuple(i, pos, dist));
        continue;
      }

      for (auto neighbor: neighbors[pos]) {
        int npos = neighbor.first, ndist = neighbor.second;
        // cout << (npos/COLCOUNT) << "," << (npos%COLCOUNT) << " at dist " << dist + ndist << endl;
        if (!visited[npos] && dist + ndist < dists[npos]) {
          dists[npos] = dist + ndist;
          working.push(make_pair(npos, dists[npos]));
        }
      }
    }
  }

  int potdist = INT_MAX;
  // otherwise, try out the possible keys
  while (!reachable_keys.empty()) {
    int quad, pos, dist;
    tie(quad, pos, dist) = reachable_keys.top();
    reachable_keys.pop();

    char key = room[pos];
    bool for_door = (doors.find(key) != doors.end());
    // cout << "Take key " << key << " which is " << dist << " away" << endl;

    int start_pos = start_poses[quad];
    room[start_pos] = '.';
    room[pos] = '@';
    if (for_door) room[doors[key]] = '.';
    keysleft.erase(key);
    start_poses[quad] = pos;

    int dist_left = recurse(room, neighbors, doors, memoizer, keysleft, start_poses);
    if (dist_left + dist < potdist) potdist = dist_left + dist;

    start_poses[quad] = start_pos;
    keysleft.insert(key);
    room[pos] = key;
    if (for_door) room[doors[key]] = toupper(key);
    room[start_pos] = '@';
  }

  memoizer[make_pair(start_poses, keysleft)] = potdist;
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
  for (int j = 0; j < ROWCOUNT; j++)
    strncpy(room + j*COLCOUNT, raw[j].c_str(), COLCOUNT);

  int start_pos;
  for (int j = 0; j < ROWCOUNT*COLCOUNT; j++) if (room[j] == '@') start_pos = j;
  array<int, 4> start_poses = {start_pos - COLCOUNT - 1, start_pos - COLCOUNT + 1,
                               start_pos + COLCOUNT - 1, start_pos + COLCOUNT + 1};
  room[start_pos] = '#';
  room[start_pos + 1] = '#';
  room[start_pos - 1] = '#';
  room[start_pos + COLCOUNT] = '#';
  room[start_pos - COLCOUNT] = '#';
  for (int start_pose : start_poses) room[start_pose] = '@';

  unordered_map<int, int> neighbors[ROWCOUNT*COLCOUNT];
  unordered_map<char, int> alphalocs;
  for (int j = 0; j < ROWCOUNT*COLCOUNT; j++) {
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

  unordered_map<pair<array<int, 4>, set<char>>, int, MemoHasher> memoizer;
  set<char> keysleft;
  for (auto elem : alphalocs) if (islower(elem.first)) keysleft.insert(elem.first);

  cout << "Total steps needed: " << recurse(room, neighbors, doors, memoizer, keysleft, start_poses) << endl;

  return 0;
}