#include <iostream>
#include <set>
#include <random>
#include <iomanip>

using namespace std;

#define REALMAX 1000
#define EPS 1e-5

uniform_real_distribution<> realgen(0, REALMAX);
uniform_int_distribution<> intgen(0, REALMAX);

bool realCmp(double a, double b) {
  return b - a > EPS;
}

bool realPairCmp(
    const pair<double, double> &a,
    const pair<double, double> &b) {
  if(realCmp(a.first, b.first)) return true;
  if(realCmp(b.first, a.first)) return false;

  return realCmp(a.second, b.second);
}

typedef struct realPairLess {
  bool operator()(
      const pair<double, double> &a,
      const pair<double, double> &b) const {
    return realPairCmp(a, b);
  }
} realPairLess;

set<pair<double, double>, realPairLess> points;

pair<double, double> mkPoint(mt19937 &gen, bool isReal) {
  pair<double, double> result;
  while(true) {
    if(isReal) result = make_pair(realgen(gen), realgen(gen));
    else result = make_pair<double, double>(intgen(gen), intgen(gen));

    if(points.count(result) == 0) {
      points.insert(result);
      return result;
    }
  }
}

int main() {
  random_device rd;
  mt19937 gen(rd());

  int n;
  cin>>n;

  bool isA;
  cin>>isA;

  double d = isA ? intgen(gen) : realgen(gen);

  cout<<fixed<<setprecision(5);

  cout<<n<<" "<<d<<endl;

  for(int i = 0; i < n; ++i) {
    const auto point = mkPoint(gen, !isA);
    cout<<point.first<<" "<<point.second<<" "<<realgen(gen)<<endl;
  }
}
