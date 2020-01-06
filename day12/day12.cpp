#include <bits/stdc++.h>
using namespace std;

struct PosVelHasher {
  size_t operator()(const pair<array<int, 4>, array<int, 4>>& posvel) const {
    size_t h = 0;
    for (size_t i = 0; i < 4; ++i) h = h*31 + posvel.first[i];
    for (size_t i = 0; i < 4; ++i) h = h*31 + posvel.second[i];
    return h;
  }
};

template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

long gcd(long u, long v) {
  return (v != 0)? gcd(v, u % v) : u;
}

long lcm(long u, long v) {
  return u/gcd(u, v)*v;
}

int main() {
  // below is copied from input data
  int moonpos[4][3] = {{12, 0, -15}, {-8, -5, -10}, {7, -17, 1}, {2, -11, -6}};

  pair<array<int, 4>, array<int, 4>> posvelaxes[3];
  for (int axis = 0; axis < 3; axis++) {
    for (int moon = 0; moon < 4; moon++) {
      posvelaxes[axis].first[moon] = moonpos[moon][axis];
      posvelaxes[axis].second[moon] = 0;
    }
  }

  bool notdone = true;
  long found[3] = {0, 0, 0};
  unordered_set<pair<array<int, 4>, array<int, 4>>, PosVelHasher> stores[3];
  for (int j = 0; notdone; j++) {
    if (j == 1000) {
      // Calculate energy
      int energy = 0;
      for (int moon = 0; moon < 4; moon++) {
        int pot = 0, kin = 0;
        for (int axis = 0; axis < 3; axis++) {
          pot += abs(posvelaxes[axis].first[moon]);
          kin += abs(posvelaxes[axis].second[moon]);
        }
        energy += pot*kin;
      }
      cout << "Total Energy after 1000 iterations: " << energy << endl;
    }

    // for each of X Y Z systems store configuration in hashset
    // when a repeat situation is found for all 3, done
    notdone = false;
    for (int axis = 0; axis < 3; axis++) {
      if (found[axis] <= 0) {
        if (stores[axis].find(posvelaxes[axis]) != stores[axis].end()) found[axis] = j;
        else {
          stores[axis].insert(posvelaxes[axis]);
          notdone = true;
        }
      }
    }

    // Apply gravity to velocity
    for (int moon1 = 0; moon1 < 3; moon1++) {
      for (int moon2 = moon1 + 1; moon2 < 4; moon2++) {
        for (int axis = 0; axis < 3; axis++) {
          int delta = sgn(posvelaxes[axis].first[moon2] - posvelaxes[axis].first[moon1]);
          posvelaxes[axis].second[moon1] += delta;
          posvelaxes[axis].second[moon2] -= delta;
        }
      }
    }
    // Apply velocity to position
    for (int moon = 0; moon < 4; moon++) {
      for (int axis = 0; axis < 3; axis++) {
        posvelaxes[axis].first[moon] += posvelaxes[axis].second[moon];
      }
    }
  }

  cout << "Periods in X Y Z: ";
  cout << found[0] << " " << found[1] << " " << found[2] << endl;

  cout << "X Y Z are independent systems so overall period is LCM of their periods:" << endl;
  cout << lcm(found[0], lcm(found[1], found[2])) << endl;

  return 0;
}