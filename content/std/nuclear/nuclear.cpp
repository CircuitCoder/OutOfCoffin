#include <cstdio>
// #include <iostream>
#include <algorithm>

using namespace std;

#define NMAX 200000 // 200K
#define EPS 1e-5

#define MAX(a,b) ((a)>(b)?(a):(b))

typedef struct Target {
  double x;
  double y;
  double v;

  bool operator<(const struct Target &ano) const {
    return ano.x - x > EPS;
  }
} Target;

Target targets[NMAX]; // 24 Bytes
double ymap[NMAX];

class SegNode {
public:
  int f, t; // 8 Bytes
  SegNode *l, *r; // 8 Bytes
  double delta = 0; // 4 Bytes
  double max = 0; // 4 Bytes

  void modify(int b, int e, double value) {
//    cout<<"Modifying: ["<<b<<", "<<e<<"] delta: "<<value<<endl;
//    cout<<"\t@"<<f<<" -> "<<t<<endl;
    if(b > t || e < f) return;
    else if(b <= f && e >= t)
      delta += value;
    else {
      l->modify(f, t, delta);
      r->modify(f, t, delta);

      delta = 0;

      l->modify(b, e, value);
      r->modify(b, e, value);
    }

    if(f == t) max = delta;
    else max = delta + MAX(l->max, r->max);
//    cout<<"\tUpdated: "<<max<<" "<<delta<<endl;
  }
};

SegNode nodepool[NMAX * 2 + 100];
int pooltop = 0;

SegNode* buildSeg(int f, int t) {
  SegNode *n = nodepool + pooltop++;
  n->f = f;
  n->t = t;

  if(f != t) {
    const int mid = (f+t)>>1;
    n->l = buildSeg(f, mid);
    n->r = buildSeg(mid + 1, t);
  }

  return n;
}

bool realcmp(double a, double b) {
  return b - a > EPS;
}

int main() {
  freopen("/Users/CircuitCoder/Workspace/OutOfCoffin/content/nuclear.in", "r", stdin);
  int n;
  double d;
  scanf("%d%lf", &n, &d);

  for(int i = 0; i<n; ++i)
    scanf("%lf%lf%lf",
        &targets[i].x,
        &targets[i].y,
        &targets[i].v);

  for(int i = 0; i<n; ++i)
    ymap[i] = targets[i].y;

  sort(targets, targets + n);
  sort(ymap, ymap + n, realcmp);

  Target *enterPtr = targets,
         *leavePtr = targets;

  SegNode *seg = buildSeg(0, n-1);

  double result = 0;

  while(enterPtr < targets + n) {
    const int head = lower_bound(ymap, ymap + n, enterPtr->y - d) - ymap;
    const int tail = upper_bound(ymap, ymap + n, enterPtr->y) - 1 - ymap;

    seg->modify(head, tail, enterPtr->v);
//    cout<<leavePtr->x<<"\t"<<enterPtr->x-d<<endl;

    while(leavePtr < enterPtr && realcmp(leavePtr->x, enterPtr->x - d)) {
      const int lhead
        = lower_bound(ymap, ymap + n, leavePtr->y - d) - ymap;
      const int ltail
        = upper_bound(ymap, ymap + n, leavePtr->y) - 1 - ymap;

      seg->modify(lhead, ltail, -leavePtr->v);

      ++leavePtr;
    }

    ++enterPtr;

    if(result < seg->max) result = seg->max;
  }

  printf("%l.5f", result);
}
