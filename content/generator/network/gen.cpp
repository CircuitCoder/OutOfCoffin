#include <iostream>
#include <random>
#include <unordered_set>

#define MMAX 2000000
#define NMAX 2000000

using namespace std;

unordered_set<uint64_t> registry;

void reg(uint64_t f, uint64_t t) {
  registry.insert(f << 32 | t);
  registry.insert(t << 32 | f);
}

bool check(uint64_t f, uint64_t t) {
  return registry.count(f << 32 | t) > 0;
}

class UFSet {
public:
  UFSet *root = nullptr;

  UFSet* getRoot() {
    if(root) return root = root->getRoot();
    else return this;
  }

  void merge(UFSet *s) {
    if(s->getRoot() == this->getRoot()) return;
    s->getRoot()->root = this->getRoot();
  }
};

UFSet nodes[NMAX];
pair<int, int> roads[MMAX];

int main() {
  random_device rd;
  mt19937 gen(rd());

  int n, m;
  cin>>n>>m;

  // TODO: isA?

  uniform_int_distribution<> nodeDist(1, n);

  while(true) {
    int f = nodeDist(gen);
    int t = nodeDist(gen);
    while(t == f) t = nodeDist(gen);
    
    for(int i = 0; i<m;) {
      int from = nodeDist(gen),
          to = nodeDist(gen);
      while(to == from) to = nodeDist(gen);

      if(check(from, to)) continue;

      nodes[from-1].merge(nodes+to-1);

      roads[i] = { from, to };

      ++i;
    }

    if(nodes[f].getRoot() == nodes[t].getRoot()) {
      // Correct
      cout<<n<<" "<<m<<endl;
      cout<<f<<" "<<t<<endl;
      for(int i = 0; i<m; ++i)
        cout<<roads[i].first<<" "<<roads[i].second<<endl;
      break;
    }

    for(int i = 0; i<n; ++i) nodes[i].root = nullptr;
    registry.clear();
  }
}
