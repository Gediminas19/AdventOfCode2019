#include <bits/stdc++.h>
using namespace std;

typedef struct asteroid asteroid;
struct asteroid{
  int x;
  int y;
};

typedef struct dasteroid dasteroid;
struct dasteroid{
  int dx;
  int dy;
};

bool clockwise(dasteroid lhs, dasteroid rhs) {
  return lhs.dx*rhs.dy > lhs.dy*rhs.dx;
}

bool clockwisee(dasteroid lhs, dasteroid rhs) {
  return clockwise(lhs, rhs) || (lhs.dx == rhs.dx && lhs.dy == rhs.dy);
}

struct comp_angle
{
  template<typename T>
  bool operator()(const T &lhs, const T &rhs) const {
    dasteroid up = {0, -1};
    dasteroid down = {0, 1};

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

  vector<asteroid> asteroids;

  string row;
  int rowcount = 0;
  while (getline(cin, row)) {
    for(int i = 0; i < row.size(); i++) {
      if (row[i] == '#') {
        asteroids.push_back(asteroid {i, rowcount});
      }
    }
    rowcount++;
  }

  map<dasteroid, set<int>, comp_angle> topviews;
  asteroid best;
  for (asteroid rock : asteroids) {
    // cout << "Base: " << rock.x << "," << rock.y << endl;
    map<dasteroid, set<int>, comp_angle> views;
    for (asteroid view : asteroids) {
      int dx = view.x - rock.x;
      int dy = view.y - rock.y;
      int f = gcd(abs(dx), abs(dy));
      if (dx == 0 && dy == 0) continue;

      if (dx == 0) {
        views[dasteroid {0, sgn(dy)}].insert(abs(dy));
      } else {
        if (dy == 0) views[dasteroid {sgn(dx), 0}].insert(abs(dx));
        else views[dasteroid {dx/f, dy/f}].insert(f);
      }
    }

    if (views.size() > topviews.size()) {
      best = rock;
      topviews = views;
    }
  }
  cout << topviews.size() << " viewable from " << best.x << "," << best.y << endl;


  int j = 1;
  bool empty = false;
  while (!empty) {
    empty = true;
    for (auto &element : topviews) {
      if (!element.second.empty()) {
        int checkx = best.x + element.first.dx*(*element.second.begin());
        int checky = best.y + element.first.dy*(*element.second.begin());
        // if (j == 200)
          cout << "asteroid " << j << " removed:: (" << checkx << "," << checky << ")" << endl;
        element.second.erase(element.second.begin());
        empty = false;
        j++;
      }
    }
  }


  return 0;
}