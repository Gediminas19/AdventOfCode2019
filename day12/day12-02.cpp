#include <bits/stdc++.h>
using namespace std;

namespace std {
  template<typename T, size_t N>
  struct hash<array<T, N>>
  {
    size_t operator()(const array<T, N>& a) const {
      hash<T> hasher;
      size_t h = 0;
      for (size_t i = 0; i < N; ++i) {
          h = h * 31 + hasher(a[i]);
      }
      return h;
    }
  };
}

long gcd(long u, long v) {
  return (v != 0)? gcd(v, u % v) : u;
}

long lcm(long u, long v) {
  return u/gcd(u, v)*v;
}

int main() {
  // int moonpos[4][3] = {{-1, 0, 2}, {2, -10, -7}, {4, -8, 8}, {3, 5, -1}};
  int moonpos[4][3] = {{12, 0, -15}, {-8, -5, -10}, {7, -17, 1}, {2, -11, -6}};
  int moonvel[4][3] = {{0}};

  bool notdone = true;
  long found[3] = {0};
  unordered_set<array<int, 8>> stores[3];
  for (int j = 0; notdone; j++) {
    // for each of X Y Z systems store configuration in hashset
    // when a repeat situation is found for all 3, done
    notdone = false;
    for (int r = 0; r < 3; r++) {
      array<int, 8> sn;
      for (int p = 0; p < 4; p++) {
        sn[2*p] = moonpos[p][r];
        sn[2*p + 1] = moonvel[p][r];
      }
      if (found[r] <= 0) {
        if (stores[r].find(sn) != stores[r].end()) found[r] = j;
        else {
          stores[r].insert(sn);
          notdone = true;
        }
      }
    }

    // Apply gravity to velocity
    for (int p = 0; p < 3; p++) {
      for (int q = p + 1; q < 4; q++) {
        for (int r = 0; r < 3; r++) {
          int delta = (moonpos[p][r] < moonpos[q][r])? 1 : ((moonpos[p][r] == moonpos[q][r])? 0 : -1);
          moonvel[p][r] += delta;
          moonvel[q][r] -= delta;
        }
      }
    }
    // Apply velocity to position
    for (int p = 0; p < 4; p++) {
      for (int r = 0; r < 3; r++) {
        moonpos[p][r] += moonvel[p][r];
      }
    }
  }
  cout << "Periods in X Y Z: ";
  cout << found[0] << " " << found[1] << " " << found[2] << endl;

  cout << "X Y Z are independent systems, overall period is LCM of their periods." << endl;
  cout << lcm(found[0], lcm(found[1], found[2])) << endl;

  return 0;
}