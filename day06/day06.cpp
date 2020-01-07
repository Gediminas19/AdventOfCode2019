#include <bits/stdc++.h>
using namespace std;

typedef struct node node;
struct node{
  node *parent;
  vector<node*> children;
  string id;
};

int getCumDepth(node* pos, int depth) {
  int temp = depth;
  for (node* kid : pos->children) {
    temp += getCumDepth(kid, depth + 1);
  }
  return temp;
}

node *newNode(string id) {
  node *res = new node;
  res->id = id;
  res->parent = NULL;
}


int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  fstream fp;
  fp.open("input.txt");

  unordered_map<string, node*> dict;

  string parent, child;
  while (!fp.eof()) {
    getline(fp, parent, ')');
    getline(fp, child);

    if (dict.find(parent) == dict.end()) {
      dict[parent] = newNode(parent);
    }
    if (dict.find(child) == dict.end()) {
      dict[child] = newNode(child);
    }

    dict[child]->parent = dict[parent];
    dict[parent]->children.push_back(dict[child]);
  }
  fp.close();

  int totalOrbits = getCumDepth(dict["COM"], 0);
  cout << "Total Orbits: " << totalOrbits << endl;

  vector<node*> sanpath;
  node* curr = dict["SAN"]->parent;
  while (curr != NULL) {
    sanpath.push_back(curr);
    curr = curr->parent;
  }

  // vector<node*> youpath;
  int you_lca_dist = 0;
  curr = dict["YOU"]->parent;
  while (curr != NULL) {
    // youpath.push_back(curr);

    auto it = find_if(sanpath.begin(), sanpath.end(), [&](node* const& obj){
      return obj == curr;
    });

    if (it != sanpath.end()) {
      cout << "LCA is " << curr->id << endl;

      int san_lca_dist = distance(sanpath.begin(), it);
      cout << "Transfers needed: " << san_lca_dist + you_lca_dist << endl;
      break;
    }

    curr = curr->parent;
    you_lca_dist += 1;
  }

  return 0;
}