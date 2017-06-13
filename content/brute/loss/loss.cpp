#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

using namespace std;

#define NMAX 10000000
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

inline
int getInt() {
  char c = getchar();
  int result = 0;
  while(c > '9' || c < '0') c = getchar();
  while(c <= '9' && c >= '0') {
    result *= 10;
    result += c - '0';
    c = getchar();
  }
  return result;
}

const int divMap[] = {
  1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
};

inline
double getReal() {
  char c = getchar();
  double result = 0;
  bool dot = false;
  int counter = 0;
  while(c > '9' || c < '0') c = getchar();
  while(true) {
    if(c == '.') {
      dot = true;
      c = getchar();
      continue;
    } else if(c > '9' || c < '0') break;

    result *= 10;
    result += c - '0';
    if(dot) ++counter;
    c = getchar();
  }
  return result / divMap[counter];
}

Event events[MMAX*2];

int main() {
  int n = getInt(),m = getInt();
  for(int i = 0; i<n; ++i)
    nodes[i].value = getReal();

  int a, b;
  for(int i = 1; i<n; ++i) {
    a = getInt() - 1;
    b = getInt() - 1;
    nodes[a].links.push_back(nodes + b);
    nodes[b].links.push_back(nodes + a);
  }

  nodes[0].dfs(1, nullptr);

  double tf, tt;
  int nf, nt;
  for(int i = 0; i<m; ++i) {
    nf = getInt() - 1;
    nt = getInt() - 1;
    tf = getReal();
    tt = getReal();

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
