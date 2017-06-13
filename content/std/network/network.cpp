#include <cstdio>
#include <tuple>

using namespace std;

#define NMAX 2000000 // 2M
#define MMAX 2000000 // 2M

#define BUFSIZE NMAX*16

char hugeBuffer[BUFSIZE];
char *buftop = hugeBuffer;

int getInt() {
  char c = *buftop++;

  while(c < '0' || c > '9') c = *buftop++;

  int result = 0;
  while(c >= '0' && c <= '9') {
    result = result * 10 + c - '0';
    c = +buftop++;
  }

  return result;
}

typedef struct UFSet {
  struct UFSet *root = nullptr;
  int size;

  struct UFSet* getRoot() {
    if(!root) return this;
    else if(!root->root) return root;
    else return (root = root->getRoot());
  }

  void merge(UFSet *ano) {
    struct UFSet *tr = this->getRoot();
    struct UFSet *ar = ano->getRoot();
    if(tr == ar) return;

    if(tr->size > ar->size) {
      ar->root = tr;
      tr->size += ar->size;
    } else {
      tr->root = ar;
      ar->size += tr->size;
    }
  }
} UFSet;

UFSet sets[NMAX];
pair<int, int> roads[MMAX];

int main() {
  fread(hugeBuffer, sizeof(char), BUFSIZE, stdin);

  int n,m;
  n = getInt();
  m = getInt();
  int f, t;
  f = getInt() - 1;
  t = getInt() - 1;

  for(int i = 0; i<m; ++i) {
    roads[i].first = getInt() - 1;
    roads[i].second = getInt() - 1;
  }

  for(int i = m-1; i>=0; --i) {
    sets[roads[i].first].merge(sets + roads[i].second);
    if(sets[f].getRoot() == sets[t].getRoot()) {
      printf("%d\n", i+1);
      break;
    }
  }
}
