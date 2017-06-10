#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

#define NMAX 100000
#define MMAX 10000

class SegNode {
public:
  int f, t;
  SegNode *l, *r;

  double size = 0;
  double stat = 0;
  int cover = 0;

  static SegNode* build(int, int, double[]);

  void update() {
    if(cover) stat = size;
    else if(f == t) stat = 0;
    else stat = l->size + r->size;
  }

  void pushdown() {
    l->put(f, (f+t)>>1, cover);
    r->put(1 + ((f+t)>>1), t, cover);
    cover = 0;
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

    const int mid = (f+t) >> 1;

    if(b <= mid)
      l->put(b, min(mid, e), delta);
    if(e > mid)
      r->put(max(mid+1, b), e, delta);

    stat = l->stat + r->stat;
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

    const int mid = (f+t) >> 1;

    if(b <= mid)
      l->remove(b, min(e, mid));
    if(e > mid)
      r->remove(max(mid+1, b), e);

    stat = l->stat + r->stat;
//    cout<<"Stat @ "<<f<<" -> "<<t<<": "<<stat<<endl;
  }
};

SegNode segpool[NMAX * 4];
int segpooltop = 0;

SegNode* SegNode::build(int from, int to, double values[]) {
  SegNode *result = segpool + segpooltop++;

  result->f = from;
  result->t = to;
  
  if(from == to) {
    result->size = values[from];
    return result;
  }

  const int mid = (from + to) >> 1;

  result->l = build(from, mid, values);
  result->r = build(mid + 1, to, values);

  result->size = result->l->size + result->r->size;
  return result;
}

class TreeNode {
public:
  double value;
  vector<TreeNode *> links;
  vector<TreeNode *> children;

  TreeNode *heavy;
  TreeNode *head;
  TreeNode *parent;
  int num;

  int size;
  int depth;
  
  int dfs(int d, TreeNode *parent) {
    size = 1;
    depth = d;
    for(auto i = links.cbegin(); i != links.cend(); ++i)
      if(*i != parent) {
        size += (*i)->dfs(d+1, this);
        children.push_back(*i);
      }
    return size;
  }

  void init(int &counter, double values[]) {
    num = counter++;
    values[num] = value;

    if(children.empty()) return;

    int maxSize = 0;
    for(auto i = children.cbegin(); i != children.cend(); ++i)
      if((*i)->size > maxSize) {
        maxSize = (*i)->size;
        heavy = *i;
      }

    heavy->head = this->head;
    heavy->parent = this->parent;
    heavy->init(counter, values);

    for(auto i = children.cbegin(); i != children.cend(); ++i)
      if(*i != heavy) {
        (*i)->head = *i;
        (*i)->parent = this;
        (*i)->init(counter, values);
      }
  }

  void linkTo(TreeNode *t) {
    t->links.push_back(this);
    links.push_back(t);
  }

  template<class CbFn>
  static void split(TreeNode *a, TreeNode *b, CbFn fn) {
    while(a->head != b->head) {
      if(a->head->depth > b->head->depth) {
        fn(a->head->num, a->num);
        a = a->parent;
      } else {
        fn(b->head->num, b->num);
        b = b->parent;
      }
    }

    int f = min(a->num, b->num);
    int t = max(a->num, b->num);
    fn(f, t);
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

double buffer[NMAX];

int n, m;

int main() {
  cin>>n>>m;
  for(int i = 0; i<n; ++i) cin>>treepool[i].value;
  int a, b;
  for(int i = 0; i<n-1; ++i) {
    cin>>a>>b;
    --a;
    --b;
    treepool[a].linkTo(treepool + b);
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
    cin>>nf>>nt>>tf>>tt;

    timeline[i * 2] = {
      tf,
      nf-1, nt-1,
      true,
    };

    timeline[i * 2 + 1] = {
      tt,
      nf-1, nt-1,
      false,
    };
  }

  sort(timeline, timeline + m * 2);

  auto addFn = [&](int f, int t) -> void {
    segRoot->put(f, t);
  };

  auto remFn = [&](int f, int t) -> void {
    segRoot->remove(f, t);
  };

  double lastEvent = 0;
  double result = 0;

  for(int i = 0; i < m*2; ++i) {
//    cout<<"Event: "<<i<<endl;
//    cout<<"Current stat: "<<segRoot->stat<<endl;
    const auto ev = timeline[i];
    result += (ev.time - lastEvent) * segRoot->stat;
    lastEvent = ev.time;
    if(ev.isAdd)
      TreeNode::split(treepool + ev.nodeF, treepool + ev.nodeT, addFn);
    else
      TreeNode::split(treepool + ev.nodeF, treepool + ev.nodeT, remFn);
  }

  cout<<fixed<<setprecision(5)<<result<<endl;
}
