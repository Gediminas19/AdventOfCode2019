#include <bits/stdc++.h>
using namespace std;

long calculate(unordered_map<string, pair<long, vector<pair<long, string>>>> reactions, long fuel, bool debug) {
  unordered_map<string, long> requirements, nrequirements;
  requirements["FUEL"] = fuel;
  bool done = false;
  while (!done) {
    done = true;
    for (auto elem : requirements) {
      if (elem.second > 0) {
        if (debug) cout << elem.second << " " << elem.first << endl;
        if (elem.first.compare("ORE") != 0) done = false;
      }
    }
    if (debug) cout << endl;

    nrequirements = requirements;
    for (auto& elem : requirements) {
      if (elem.first.compare("ORE") == 0 || elem.second <= 0) continue;

      long needed = requirements[elem.first];
      long produced = reactions[elem.first].first;
      long q = (needed + produced - 1) / produced;

      for (auto precursor : reactions[elem.first].second) {
        long preneeded = precursor.first;
        string prename = precursor.second;
        if (nrequirements.find(prename) == nrequirements.end()) nrequirements[prename] = preneeded*q;
        else nrequirements[prename] += preneeded*q;
      }
      nrequirements[elem.first] -= (produced*q);
    }
    requirements = nrequirements;
  }

  return requirements["ORE"];
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  long reacount;
  scanf("%ld\n", &reacount);

  unordered_map<string, pair<long, vector<pair<long, string>>>> reactions;
  for (long j = 0; j < reacount; j++) {
    vector<pair<long, string>> reactants;
    long reamount;
    char reactant[10];
    while (scanf("%ld %s", &reamount, reactant) == 2) {
      reactants.push_back(make_pair(reamount, string(reactant)));
    }
    scanf("=>");

    long pramount;
    char product[10];
    scanf("%ld %s", &pramount, product);
    reactions[string(product)] = make_pair(pramount, reactants);
  }

  // Part 1
  cout << calculate(reactions, 1, false) << " ore needed for 1 fuel" << endl << endl;

  // Part 2
  long goal = 1000000000000;

  long island = 0;
  long bot = 0;
  long top = 2000000;
  long mid = 0;
  while (bot < top) {
    mid = bot + (top - bot)/2;

    island = calculate(reactions, mid, false);
    if (island < goal) bot = mid + 1;
    else if (island > goal) top = mid;

    cout << mid << " fuel requires " << island << " ore" << endl;
  }

  long ans = (island <= goal)? mid : mid - 1;
  cout << ans << " fuel produceable from " << goal << " ore via::" << endl << endl;
  calculate(reactions, ans, true);

  return 0;
}