#include <bits/stdc++.h>
using namespace std;

int main() {
  ifstream fp;
  fp.open("input.txt");

  int dim = 5;
  char **eris = new char*[dim];
  string read;
  for (int j = 0; j < dim; j++) {
    eris[j] = new char[dim];
    getline(fp, read);
    strcpy(eris[j], read.c_str());
  }
  fp.close();

  int **eris_count = new int*[dim + 2];
  for (int j = 0; j < dim + 2; j++) eris_count[j] = new int[dim + 2]();

  unordered_set<int> biodiversities;
  while (true) {
    // save layout
    int biodiversity = 0, temp = 1;
    for (int j = 0; j < dim; j++) {
      for (int k = 0; k < dim; k++) {
        if (eris[j][k] == '#') biodiversity += temp;
        temp *= 2;
      }
    }
    if (biodiversities.find(biodiversity) != biodiversities.end()) {
      cout << biodiversity << " repeated" << endl;
      break;
    }
    biodiversities.insert(biodiversity);


    // calculate neighbor bug counts
    for (int j = 0; j < dim; j++) {
      for (int k = 0; k < dim; k++) {
        if (eris[j][k] == '#') {
          eris_count[j][k + 1] += 1;
          eris_count[j + 1][k] += 1;
          eris_count[j + 1][k + 2] += 1;
          eris_count[j + 2][k + 1] += 1;
        }
      }
    }

    // apply neighbor bug count rules to update layout
    for (int j = 0; j < dim; j++) {
      for (int k = 0; k < dim; k++) {
        if (eris[j][k] == '#' && eris_count[j + 1][k + 1] != 1) eris[j][k] = '.';
        else if (eris[j][k] == '.' && (eris_count[j + 1][k + 1] == 1
                || eris_count[j + 1][k + 1] == 2)) eris[j][k] = '#';
        eris_count[j + 1][k + 1] = 0;
      }
    }
  }

  // for (int j = 0; j < dim; j++) {
  //   for (int k = 0; k < dim; k++) {
  //     cout << eris[j][k];
  //   }
  //   cout << endl;
  // }


  return 0;
}