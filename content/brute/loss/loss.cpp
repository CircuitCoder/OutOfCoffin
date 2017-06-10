#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

using namespace std;

#define NMAX 100000
#define MMAX 100000

class TreeNode {
public:
  vector<TreeNode *> links;
  double value;
  int cover = 0;

  int depth;
  TreeNode *parent;

  void dfs(int d, TreeNode *p) {
    parent = p;
    depth = d;
    for(auto i = links.begin(); i<links.end(); ++i)
      if(*i != p)
        (*i) -> dfs(d+1, this);
  }
};

double stash = 0;

void put(TreeNode *f, TreeNode *t) {
  while(f != t)
    if(f->depth > t->depth) {
      if(f->cover++ == 0)
        stash += f->value;
      f = f->parent;
    } else {
      if(t->cover++ == 0)
        stash += t->value;
      t = t->parent;
    }

  if(f->cover++ == 0)
    stash += f->value;
}

void remove(TreeNode *f, TreeNode *t) {
  while(f != t)
    if(f->depth > t->depth) {
      if(--f->cover == 0)
        stash -= f->value;
      f = f->parent;
    } else {
      if(--t->cover == 0)
        stash -= t->value;
      t = t->parent;
    }

  if(--f->cover == 0)
    stash -= f->value;
}

TreeNode nodes[NMAX];

typedef struct Event {
  double ts;
  int f, t;
  bool isAdd;

  bool operator<(const Event &ano) const {
    return ts < ano.ts;
  }
} Event;

Event events[MMAX*2];

int main() {
  int n,m;
  cin>>n>>m;
  for(int i = 0; i<n; ++i)
    cin>>nodes[i].value;

  int a, b;
  for(int i = 1; i<n; ++i) {
    cin>>a>>b;
    --a;
    --b;
    nodes[a].links.push_back(nodes + b);
    nodes[b].links.push_back(nodes + a);
  }

  nodes[0].dfs(1, nullptr);

  double tf, tt;
  int nf, nt;
  for(int i = 0; i<m; ++i) {
    cin>>nf>>nt>>tf>>tt;
    --nt;
    --nf;
    events[i*2] = {
      tf,
      nf, nt,
      true,
    };

    events[i*2+1] = {
      tt,
      nf, nt,
      false,
    };
  }

  sort(events, events + m*2);

  double lastTs = 0;
  double result = 0;
  for(int i = 0; i<m*2; ++i) {
    result += (events[i].ts - lastTs) * stash;
    lastTs = events[i].ts;

    if(events[i].isAdd)
      put(nodes + events[i].f, nodes + events[i].t);
    else
      remove(nodes + events[i].f, nodes + events[i].t);
  }
  cout<<fixed<<setprecision(5)<<result<<endl;
}
