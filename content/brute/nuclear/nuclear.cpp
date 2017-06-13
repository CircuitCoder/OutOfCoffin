#include <cstdio>

using namespace std;

#define NMAX 5000000
#define EPS 1e-5

bool realcmp(double a, double b) {
  return b - a > EPS;
}

typedef struct T {
  double x;
  double y;
  double v;
} Target;

Target targets[NMAX];

int main() {
  int n;
  double d;
  scanf("%d%lf", &n, &d);

  for(int i = 0; i<n; ++i)
    scanf("%lf%lf%lf",
        &targets[i].x,
        &targets[i].y,
        &targets[i].v);

  double result = 0;

  for(int top = 0; top < n; ++top)
    for(int right = 0; right < n; ++right) {
      double stash = 0;

      for(int counter = 0; counter < n; ++counter)
        if(!realcmp(targets[counter].x, targets[right].x)
            && realcmp(targets[counter].x, targets[right].x + d)
            && !realcmp(targets[counter].y, targets[top].y)
            && realcmp(targets[counter].y, targets[top].y + d))
          stash += targets[counter].v;

      if(stash > result) result = stash;
    }

  printf("%l.5f", result);
}
