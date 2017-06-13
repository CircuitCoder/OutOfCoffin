#include <iostream>
#include <unordered_set>
#include <random>
#include <algorithm>
#include <iomanip>
using namespace std;

#define NMAX 10000000
#define MMAX 100000

#define REALPRE 10000
#define REALPRENUM 4
#define REALMAX 50

pair<int, int> roads[NMAX+1];

int main() {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> realdis(0, REALMAX*REALPRE);

  cout<<fixed<<setprecision(REALPRENUM);

  int n, m;
  cin>>n>>m;
  bool isA, isB;
  cout<<n<<" "<<m<<endl;

  for(int i = 1; i<=n; ++i) {
    cout<<((double) realdis(gen))/REALPRE;
    if(i != n) cout<<" ";
  }
  cout<<endl;

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
  unordered_set<int> expandedTime;
  pair<double, double> timepairs[MMAX];
  uniform_int_distribution<> pathdis(1, n);

  for(int i = 0; i<m * 2; ++i) {
    int generated = realdis(gen);
    while(expandedTime.count(generated) > 0) generated = realdis(gen);
    time[i] = ((double) generated)/REALPRE;
    expandedTime.insert(generated);
  }

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
