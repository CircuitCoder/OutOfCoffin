#include <iostream>
#include <random>
#include <algorithm>
#include <iomanip>
using namespace std;

#define NMAX 100000
#define MMAX 100000

const double EPS = 1e-5;
int main() {
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> realdis(EPS, 50);

  cout<<fixed<<setprecision(5);

  int n, m;
  cin>>n>>m;
  bool isA, isB;
  cout<<n<<" "<<m<<endl;

  for(int i = 1; i<=n; ++i) {
    cout<<realdis(gen);
    if(i != n) cout<<" ";
  }
  cout<<endl;

  pair<int, int> roads[NMAX+1];

  if(isA) {
    for(int i = 2; i <= n; ++i)
      roads[i] = make_pair(i, i-1);
  } else {
    for(int i = 2; i <= n; ++i) {
      uniform_int_distribution<> targetdis(1, i-1);
      roads[i] = make_pair(i, targetdis(gen));
    }
  }

  uniform_int_distribution<> swapdis(0, 1);
  for(int i = 2; i <= n; ++i)
    if(swapdis(gen) == 1) swap(roads[i].first, roads[i].second);
  shuffle(roads+2, roads+n+1, gen);

  for(int i = 2; i <= n; ++i)
    cout<<roads[i].first<<" "<<roads[i].second<<endl;

  double time[MMAX * 2];
  pair<double, double> timepairs[MMAX];
  uniform_int_distribution<> pathdis(1, n);

  for(int i = 0; i<m * 2; ++i)
    time[i] = realdis(gen);

  if(isB)
    sort(time, time + 2*m);
  for(int i = 0; i<m; ++i) {
    timepairs[i] = make_pair(time[i*2], time[i*2+1]);
    if(timepairs[i].first > timepairs[i].second)
      swap(timepairs[i].first,
           timepairs[i].second);
  }
  shuffle(timepairs, timepairs+m, gen);
  for(int i = 0; i<m; ++i) {
    int from = pathdis(gen);
    int to = pathdis(gen);
    while(to == from) to = pathdis(gen);

    cout<<from<<" "<<to<<" "<<timepairs[i].first<<" "<<timepairs[i].second<<endl;
  }
}
