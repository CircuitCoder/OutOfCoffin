#include <iostream>
#include <vector>

using namespace std;

#define NMAX 10000000
#define MMAX 10000000

class Node {
public:
  vector<Node *> conn;

  int store = 0;

  void flood(int value) {
    if(store == value) return;

    store = value;
    for(auto i = conn.begin(); i < conn.end(); ++i)
      (*i)->flood(value);
  }
};

Node nodes[NMAX];
pair<int, int> roads[NMAX];

int main() {
  int n,m;
  int f,t;
  cin>>n>>m;
  cin>>f>>t;

  --f;
  --t;

  for(int i = 0; i<m; ++i) {
    cin>>roads[i].first>>roads[i].second;
    --roads[i].first;
    --roads[i].second;
  }

  int k = 0;
  for(int i = m-1; i>=0; --i) {
    nodes[roads[i].first].conn.push_back(nodes + roads[i].second);
    nodes[roads[i].second].conn.push_back(nodes + roads[i].first);

    nodes[f].flood(++k);
    if(nodes[t].store == k) {
      cout<<i+1<<endl;
      break;
    }
  }
}
