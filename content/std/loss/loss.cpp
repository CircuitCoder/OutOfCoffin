#include <iostream>
#include <cstdio>
#include <iomanip>
#include <cassert>

using namespace std;

#define NMAX 10000000
#define MMAX 100000

class SegNode {
public:
  int f, t;
  int mid;
  SegNode *l, *r;

  double size = 0;
  double stat = 0;
  int cover = 0;

  static SegNode* build(int, int, double[]);

  inline
  void pushdown() {
    l->put(f, mid, cover);
    r->put(mid+1, t, cover);
    cover = 0;
  }

  inline
  void mergeup() {
    int merged = min(l->cover, r->cover);
    if(merged) {
      l->cover -= merged;
      r->cover -= merged;
      if(!l->cover) l->restat();
      if(!r->cover) r->restat();
    }

    cover += merged;
    restat();
  }

  inline
  void restat() {
    if(cover) stat = size;
    else if(f == t) stat = 0;
    else stat = l->stat + r->stat;
  }

  void put(int b, int e, unsigned int delta = 1) {
//    cout<<"Puting: "<<b<<" -> "<<e<<endl;
//    cout<<"\t @: "<<f<<" -> "<<t<<endl;
    if(b == f && e == t) {
      cover += delta;
      stat = size;
      return;
    }

    if(cover)
      pushdown();

    if(b <= mid)
      l->put(b, min(mid, e), delta);
    if(e > mid)
      r->put(max(mid+1, b), e, delta);

    // mergeup();
    restat();
  }

  void remove(int b, int e) {
//    cout<<"Removing: "<<b<<" -> "<<e<<endl;
//    cout<<"\t @: "<<f<<" -> "<<t<<endl;
    if(b == f && e == t)
      if(cover) {
        --cover;
        if(cover == 0) {
          if(f == t) stat = 0;
          else stat = l->stat + r->stat;
        } else stat = size;
        return;
      }

    if(cover) pushdown();

    if(b <= mid)
      l->remove(b, min(e, mid));
    if(e > mid)
      r->remove(max(mid+1, b), e);

    // mergeup();
    restat();
//    cout<<"Stat @ "<<f<<" -> "<<t<<": "<<stat<<endl;
  }
};

SegNode segpool[NMAX * 2 + 100];
int segpooltop = 0;

SegNode* SegNode::build(int from, int to, double values[]) {
  SegNode *result = segpool + segpooltop++;

  const int mid = (from + to) >> 1;

  result->f = from;
  result->t = to;
  result->mid = mid;
  
  if(from == to) {
    result->size = values[from];
    return result;
  }

  result->l = build(from, mid, values);
  result->r = build(mid + 1, to, values);

  result->size = result->l->size + result->r->size;
  return result;
}

int _counter = 0;

class TreeNode;

typedef struct Link {
  TreeNode *e;
  struct Link *n;
} Link;

Link linkpool[NMAX * 2];
int linkpooltop = 0;

class TreeNode {
public:
  double value;
  Link *children = nullptr;

  TreeNode *heavy;
  TreeNode *head;
  TreeNode *parent;
  int num;

  int size;
  int depth;
  
  int dfs(int d, TreeNode *parent) {
    ++_counter;
    size = 1;
    depth = d;

    for(auto l = children; l; l = l->n) {
      if(l->e != parent)
        size += l->e->dfs(d+1, this);
      else
        l->e = children->e;
    }

    if(parent) children = children->n;

    return size;
  }

  void init(int &counter, double values[]) {
    num = counter++;
    values[num] = value;

    if(!children) return;

    int maxSize = 0;
    for(auto l = children; l; l = l->n)
      if(l->e->size > maxSize) {
        maxSize = l->e->size;
        heavy = l->e;
      }

    heavy->head = this->head;
    heavy->parent = this->parent;
    heavy->init(counter, values);

    for(auto l = children; l; l = l->n)
      if(l->e != heavy) {
        l->e->head = l->e;
        l->e->parent = this;
        l->e->init(counter, values);
      }
  }

  void linkTo(TreeNode *t) {
    auto l = linkpool + linkpooltop++;
    l->e = t;
    l->n = children;
    children = l;
  }

  static void put(SegNode *seg, TreeNode *a, TreeNode *b) {
    while(a->head != b->head) {
      if(a->head->depth > b->head->depth) {
        seg->put(a->head->num, a->num);
        a = a->parent;
      } else {
        seg->put(b->head->num, b->num);
        b = b->parent;
      }
    }

    int f = min(a->num, b->num);
    int t = max(a->num, b->num);
    seg->put(f, t);
  }

  static void remove(SegNode *seg, TreeNode *a, TreeNode *b) {
    while(a->head != b->head) {
      if(a->head->depth > b->head->depth) {
        seg->remove(a->head->num, a->num);
        a = a->parent;
      } else {
        seg->remove(b->head->num, b->num);
        b = b->parent;
      }
    }

    int f = min(a->num, b->num);
    int t = max(a->num, b->num);
    seg->remove(f, t);
  }
};

TreeNode treepool[NMAX];

typedef struct Event {
  double time;
  int nodeF, nodeT;
  bool isAdd;

  bool operator<(const struct Event &ano) const {
    return time < ano.time;
  }
}Event;

Event timeline[MMAX * 2];

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

double buffer[NMAX];

int n, m;

int main() {
  n = getInt();
  m = getInt();
  for(int i = 0; i<n; ++i) treepool[i].value = getReal();
  int a, b;
  for(int i = 0; i<n-1; ++i) {
    a = getInt() - 1;
    b = getInt() - 1;
    treepool[a].linkTo(treepool + b);
    treepool[b].linkTo(treepool + a);
  }

  TreeNode *root = treepool;

  double tf, tt;
  int nf, nt;

  root->dfs(1, nullptr);
  root->head = root;
  root->parent = NULL;
  int counter = 0;
  root->init(counter, buffer);

  SegNode *segRoot = SegNode::build(0, counter-1, buffer);

  for(int i = 0; i<m; ++i) {
    nf = getInt() - 1;
    nt = getInt() - 1;
    tf = getReal();
    tt = getReal();

    timeline[i * 2] = {
      tf,
      nf, nt,
      true,
    };

    timeline[i * 2 + 1] = {
      tt,
      nf, nt,
      false,
    };
  }

  sort(timeline, timeline + m * 2);

  double lastEvent = 0;
  double result = 0;

  for(int i = 0; i < m*2; ++i) {
//    cout<<"Event: "<<i<<endl;
//    cout<<"Current stat: "<<segRoot->stat<<endl;
    const auto ev = timeline[i];
    result += (ev.time - lastEvent) * segRoot->stat;
    lastEvent = ev.time;
    if(ev.isAdd)
      TreeNode::put(segRoot, treepool + ev.nodeF, treepool + ev.nodeT);
    else
      TreeNode::remove(segRoot, treepool + ev.nodeF, treepool + ev.nodeT);
  }

  cout<<fixed<<setprecision(5)<<result<<endl;
}
