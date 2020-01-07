#include <bits/stdc++.h>
using namespace std;

bool clockwise(pair<int, int> lhs, pair<int, int> rhs) {
  return lhs.first*rhs.second > lhs.second*rhs.first;
}

bool clockwisee(pair<int, int> lhs, pair<int, int> rhs) {
  return clockwise(lhs, rhs) || (lhs.first == rhs.first && lhs.second == rhs.second);
}

struct comp_angle
{
  bool operator()(const pair<int, int>& lhs, const pair<int, int>& rhs) const {
    pair<int, int> up = make_pair(0, -1);
    pair<int, int> down = make_pair(0, 1);

    if ((clockwisee(up, lhs) && clockwisee(up, rhs)) || (clockwisee(down, lhs) && clockwisee(down, rhs))) return clockwise(lhs, rhs);
    else return clockwisee(up, lhs);
  }
};

int gcd(int u, int v) {
  return (v != 0)? gcd(v, u % v) : u;
}

template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  fstream fp;
  fp.open("input.txt");

  vector<pair<int, int>> asteroids;

  string row;
  int rowcount = 0;
  while (getline(fp, row)) {
    for(int i = 0; i < row.size(); i++) {
      if (row[i] == '#')
        asteroids.push_back(make_pair(i, rowcount));
    }
    rowcount++;
  }
  fp.close();

  map<pair<int, int>, set<int>, comp_angle> topviews;
  pair<int, int> best;
  for (pair<int, int> rock : asteroids) {
    // cout << "Base: " << rock.x << "," << rock.y << endl;
    map<pair<int, int>, set<int>, comp_angle> views;
    for (pair<int, int> view : asteroids) {
      int dx = view.first - rock.first;
      int dy = view.second - rock.second;
      int f = gcd(abs(dx), abs(dy));
      if (dx == 0 && dy == 0) continue;

      if (dx == 0) {
        views[make_pair(0, sgn(dy))].insert(abs(dy));
      } else {
        if (dy == 0) views[make_pair(sgn(dx), 0)].insert(abs(dx));
        else views[make_pair(dx/f, dy/f)].insert(f);
      }
    }

    if (views.size() > topviews.size()) {
      best = rock;
      topviews = views;
    }
  }
  cout << topviews.size() << " viewable from " << best.first << "," << best.second << endl;


  int j = 1;
  bool empty = false;
  while (!empty) {
    empty = true;
    for (auto &element : topviews) {
      if (!element.second.empty()) {
        int f = *element.second.begin();
        int checkx = best.first + element.first.first*f;
        int checky = best.second + element.first.second*f;
        if (j == 200)
          cout << "asteroid " << j << " removed:: (" << checkx << "," << checky << ")" << endl;
        element.second.erase(element.second.begin());
        empty = false;
        j++;
      }
    }
  }


  return 0;
}