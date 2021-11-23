#include <bits/stdc++.h>
#define F first
#define S second
#define all(x) x.begin(),x.end()

using namespace std;

using ll = long long;
using pii = pair<int, int>;
const int INF = 0x3f3f3f3f;
const ll INFLL = 0x3f3f3f3f3f3f3f3fLL;
const ll MOD = (1ll<<61) - 1;

ll modMul(uint64_t a, uint64_t b){
  uint64_t l1 = (uint32_t)a, h1 = a>>32, l2 = (uint32_t)b, h2 = b>>32;
  uint64_t l = l1*l2, m = l1*h2 + l2*h1, h = h1*h2;
  uint64_t ret = (l&MOD) + (l>>61) + (h << 3) + (m >> 29) + ((m << 35) >> 3) + 1;
  ret = (ret & MOD) + (ret>>61);
  ret = (ret & MOD) + (ret>>61);
  return ret-1;
}

struct State{
  int f, g, h, pos0;
  vector<int> state;
  ll hash;
  State(){}
  State(int g0, int h0, vector<int> state0, int p0, ll hash0){
    f = g0 + h0;
    g = g0;
    h = h0;
    state = state0;
    pos0 = p0;
    hash = hash0;
  }
};

struct classcomp {
  bool operator() (const State& lhs, const State& rhs) const{
    return lhs.f > rhs.f;
  }
};

const int dx[] = { 0, 0, -1, 1};
const int dy[] = {-1, 1,  0, 0};
const string D = "LRUD";

vector<vector<int>> adj;
vector<ll> pre;
vector<pii> pos, posTo;

void buildAdj(int n, int m){
  adj.assign(n*m, vector<int>());
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      int id0 = i*m + j;
      for(int k=0; k<4; k++){
        int a = i + dx[k];
        int b = j + dy[k];
        if(a >= 0 and a < n and b >= 0 and b < m){
          int id1 = a*m + b;
          adj[id0].push_back(id1);
        }
      }
    }
  }
}

void buildPreHash(int n, int m){
  int k = n*m;
  pre.resize(k);
  ll base = k;
  ll h = 1;
  for(int i=k-1; i>=0; i--){
    pre[i] = h;
    h = modMul(h, base);
  }
}

void buildPosition(int n, int m){
  int k = n*m;
  pos.resize(k);
  posTo.resize(k);  
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      int id = i*m + j;
      pos[id] = pii(i, j);
    }
  }

  for(int id=1; id<k; id++)
    posTo[id] = pos[id-1];
  posTo[0] = pos[k-1];
}

void preBuild(int n, int m){
  buildAdj(n, m);
  buildPreHash(n, m);
  buildPosition(n, m);
}

int getPos(vector<int> &v, int x){
  for(int i=0; i<v.size(); i++){
    if(v[i] == x)
      return i;
  }
  return -1;
}

ll getHash(vector<int> &v){
  ll base = v.size();
  ll h = 0;

  for(int i=0; i<v.size(); i++){
    h = modMul(h, base) + v[i];
    if(h >= MOD)
      h -= MOD;
  }
  
  return h;
}

unordered_map<ll, int> dist;

ll getFinalHash(int n, int m){
  int k = n*m;
  vector<int> v(k, 0);
  iota(v.begin(), v.end(), 1);
  v.back() = 0;
  return getHash(v);
}

ll getNewHash(ll oldHash, int from0, int to0, int x){
  ll hash = oldHash - modMul(pre[to0], x);
  if(hash < 0)
    hash += MOD;
  
  hash = hash + modMul(pre[from0], x);
  if(hash >= MOD)
    hash -= MOD;
  return hash;
}

int getH(vector<int> v){
  int sum = 0;
  for(int i=0; i<v.size(); i++){
    pii now = pos[i];
    pii to = posTo[v[i]];
    if(v[i] != 0)
      sum += (abs(now.F - to.F) + abs(now.S - to.S));
  }
  return sum;
}

int getNewH(int h, int pos0, int posx, int x){
  int sum = h;

  {
    pii now = pos[posx];
    pii to = posTo[x];

    sum -= (abs(now.F - to.F) + abs(now.S - to.S));
  }

  //Add new
  swap(pos0, posx);
  {
    pii now = pos[posx];
    pii to = posTo[x];

    sum += (abs(now.F - to.F) + abs(now.S - to.S));
  }
  
  return sum;
}

int persitentAStar(vector<int> init, int n, int m){
  preBuild(n, m);
  auto finalHash = getFinalHash(n, m);
  
  auto initHash = getHash(init);
  dist[initHash] = 0;

  priority_queue<State, vector<State>, classcomp> pq;
  pq.emplace(dist[initHash], getH(init), init, getPos(init, 0), initHash);
  
  while(!pq.empty()){
    auto [f0, g0, h0, pos0, state0, hash0] = pq.top();
    pq.pop();
    
    if(hash0 == finalHash){
      return g0;
    }
    if(g0 > dist[hash0])
      continue;
    for(int to: adj[pos0]){
      int x = state0[to];
      ll newHash = getNewHash(hash0, pos0, to, x);
      int newH = getNewH(h0, pos0, to, x);
      // Swap
      auto new_state = state0;
      swap(new_state[pos0], new_state[to]);

      if(!dist.count(newHash) or g0 + 1 < dist[newHash]){
        dist[newHash] = g0 + 1;
        pq.emplace(dist[newHash], newH, new_state, to, newHash);
      }
    }
  }
  return -1;
}

int main() {
  ios_base::sync_with_stdio(false); cin.tie(NULL);
  int n, m;
  cin >> n >> m;
  vector<int> state(n*m);
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      cin >> state[i*m + j];
    }
  }
  
  cout << persitentAStar(state, n, m) << endl;
  return 0;
}
