#include <bits/stdc++.h>
using namespace std;

typedef struct segment {
  int x1;
  int y1;
  int x2;
  int y2;
  int cumdist;
} segment;


int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  ifstream fp;
  fp.open("input.txt");

  string line1, line2;
  getline(fp, line1);
  getline(fp, line2);
  fp.close();

  istringstream read1(line1);
  istringstream read2(line2);

  vector<segment> hor1, ver1, hor2, ver2;

  int x = 0, y = 0;
  char junk;
  char dir;
  int dist;
  int cumdist = 0;
  do {
    read1 >> dir >> dist;
    // cout << "Dir: " << dir << " Dist: " << dist << endl;
    switch (dir) {
      case 'R': {
        segment to_add = {x, y, x + dist, y, cumdist + dist};
        hor1.push_back(to_add);
        x += dist;
        cumdist += dist;
        break;
      }
      case 'L': {
        segment to_add = {x, y, x - dist, y, cumdist + dist};
        hor1.push_back(to_add);
        x -= dist;
        cumdist += dist;
        break;
      }
      case 'U': {
        segment to_add = {x, y, x, y + dist, cumdist + dist};
        ver1.push_back(to_add);
        y += dist;
        cumdist += dist;
        break;
      }
      case 'D': {
        segment to_add = {x, y, x, y - dist, cumdist + dist};
        ver1.push_back(to_add);
        y -= dist;
        cumdist += dist;
        break;
      }
    }
  } while (read1 >> junk);
  x = 0;
  y = 0;
  cumdist = 0;
  do {
    read2 >> dir >> dist;
    // cout << "Dir: " << dir << " Dist: " << dist << endl;
    switch (dir) {
      case 'R': {
        segment to_add = {x, y, x + dist, y, cumdist + dist};
        hor2.push_back(to_add);
        x += dist;
        cumdist += dist;
        break;
      }
      case 'L': {
        segment to_add = {x, y, x - dist, y, cumdist + dist};
        hor2.push_back(to_add);
        x -= dist;
        cumdist += dist;
        break;
      }
      case 'U': {
        segment to_add = {x, y, x, y + dist, cumdist + dist};
        ver2.push_back(to_add);
        y += dist;
        cumdist += dist;
        break;
      }
      case 'D': {
        segment to_add = {x, y, x, y - dist, cumdist + dist};
        ver2.push_back(to_add);
        y -= dist;
        cumdist += dist;
        break;
      }
    }
  } while (read2 >> junk);

  vector<int> interdist, intertime;
  for (segment hor : hor1) {
    for (segment ver : ver2) {
      if (min(hor.x1, hor.x2) <= ver.x1 && ver.x2 <= max(hor.x1, hor.x2)
          && min(ver.y1, ver.y2) <= hor.y1 && hor.y2 <= max(ver.y1, ver.y2)) {
          // cout << ver.x1 << " " << hor.y1 << endl;
          interdist.push_back(abs(hor.y1) + abs(ver.x1));
          intertime.push_back(hor.cumdist - abs(hor.x2 - ver.x1) + ver.cumdist - abs(ver.y2 - hor.y1));
      }
    }
  }
  for (segment hor : hor2) {
    for (segment ver : ver1) {
      if (min(hor.x1, hor.x2) <= ver.x1 && ver.x2 <= max(hor.x1, hor.x2)
          && min(ver.y1, ver.y2) <= hor.y1 && hor.y2 <= max(ver.y1, ver.y2)) {
          // cout << ver.x1 << " " << hor.y1 << endl;
          interdist.push_back(abs(hor.y1) + abs(ver.x1));
          intertime.push_back(hor.cumdist - abs(hor.x2 - ver.x1) + ver.cumdist - abs(ver.y2 - hor.y1));
      }
    }
  }
  interdist.erase(remove(interdist.begin(), interdist.end(), 0), interdist.end());
  intertime.erase(remove(intertime.begin(), intertime.end(), 0), intertime.end());

  cout << "Distance to closest intersection: " << *min_element(interdist.begin(), interdist.end()) << endl;
  cout << "Fewest combined steps to intersection: " << *min_element(intertime.begin(), intertime.end()) << endl;

  return 0;
}