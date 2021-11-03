#include <bits/stdc++.h>
#define F first
#define S second
#define all(x) x.begin(),x.end()

using namespace std;

using ll = long long;
using pii = pair<int, int>;
const int INF = 0x3f3f3f3f;
const ll INFLL = 0x3f3f3f3f3f3f3f3fLL;
const ll MOD = 1e9 + 7;

class PersitentSegmentTree{
private:
  vector<int> seg;
  vector<int> T, L, R;
  int cnt, t, n;
  void build(vector<int> &v, int p, int l, int r){
    if (l == r){
      seg[p] = v[l];
      return;
    }
    L[p] = cnt;
    L.emplace_back(), R.emplace_back(), seg.emplace_back(), cnt++;
    R[p] = cnt;
    L.emplace_back(), R.emplace_back(), seg.emplace_back(), cnt++;

    int mid = (l + r)>>1;
    build(v, L[p], l, mid);
    build(v, R[p], mid + 1, r);
  }
  int query(int idx, int p, int l, int r){
    if (l == r)
      return seg[p];
    int mid = (l + r)>>1;
    if (idx <= mid)
      return query(idx, L[p], l, mid);
    else
      return query(idx, R[p], mid + 1, r);
  }
  void update(int idx, int x, int lp, int p, int l, int r){
    if (l == r){
      seg[p] = x;
      return;
    }
    int mid = (l + r) / 2;
    if (idx <= mid){
      L[p] = cnt;
      L.emplace_back(), R.emplace_back(), seg.emplace_back(), cnt++;
      
      R[p] = R[lp];
      update(idx, x, L[lp], L[p], l, mid);
    }else{
      L[p] = L[lp];
      R[p] = cnt;
      L.emplace_back(), R.emplace_back(), seg.emplace_back(), cnt++;

      update(idx, x, R[lp], R[p], mid + 1, r);
    }
  }
public:
  //O(n)
  PersitentSegmentTree(vector<int> v){
    cnt = t = 0;
    n = v.size();
    T.resize(1);
    T[0] = cnt;
    L.emplace_back(), R.emplace_back(), seg.emplace_back(), cnt++;
    build(v, T[0], 0, n - 1);
  }
  int lastVersion(){
    return t;
  }
  //O(log(n))
  int get(int idx, int version){
    return query(idx, T[version], 0, n - 1);
  }
  int getLast(int idx){
    return get(idx, t);
  }
  //O(log(n))
  int update(int idx, int x, int version){
    T.emplace_back(), t++;
    T[t] = cnt;
    L.emplace_back(), R.emplace_back(), seg.emplace_back(), cnt++;
    update(idx, x, T[version], T[t], 0, n - 1);
    return t;
  }
  int updateLast(int idx, int x){
    return update(idx, x, t);
  }
  int swap(int i, int j, int version){
    int a = get(i, version);
    int b = get(j, version);
    int v = update(i, b, version);
    return update(j, a, v);
  }
  int swapLast(int i, int j){
    return swap(i, j, t);
  }
}; // namespace perseg

struct State{
  int d, version, pos0;
  ll hash;
  State(){}
  State(int d0, int version0, int p0, ll hash0){
    d = d0;
    version = version0;
    pos0 = p0;
    hash = hash0;
  }
};

struct classcomp {
  bool operator() (const State& lhs, const State& rhs) const{
    return lhs.d > rhs.d;
  }
};

const int dx[] = { 0, 0, -1, 1};
const int dy[] = {-1, 1,  0, 0};
const string D = "LRUD";
vector<vector<int>> buildAdj(int n, int m){
  vector<vector<int>> adj(n*m, vector<int>());
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
  return adj;
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
    h = (h*base)%MOD + v[i];
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

vector<ll> pre;
void buildPreHash(int k){
  pre.resize(k);
  ll base = k;
  ll h = 1;
  for(int i=k-1; i>=0; i--){
    pre[i] = h;
    h = (h*base)%MOD;
  }
}

ll getNewHash(ll oldHash, int from0, int to0, int x){
  ll hash = (oldHash - (pre[to0]*x)%MOD + (pre[from0]*x)%MOD)%MOD;
  if(hash < 0)
    hash += MOD;
  return hash;
}

int persitentDijkstra(vector<int> init, int n, int m){
  buildPreHash(n*m);
  auto finalHash = getFinalHash(n, m);
  vector<vector<int>> adj = buildAdj(n, m);
  
  PersitentSegmentTree pst(init);

  auto initHash = getHash(init);
  dist[initHash] = 0;

  priority_queue<State, vector<State>, classcomp> pq;
  pq.emplace(dist[initHash], pst.lastVersion(), getPos(init, 0), initHash);
  
  while(!pq.empty()){
    auto [d0, version0, pos0, hash0] = pq.top();
    pq.pop();
    
    if(hash0 == finalHash){
      return d0;
    }
    if(d0 > dist[hash0])
      continue;
    for(int to: adj[pos0]){
      //swap
      int x = pst.get(to, version0);
      int version = pst.update(to, 0, version0);
      version = pst.update(pos0, x, version);
      
      ll newHash = getNewHash(hash0, pos0, to, x);
      if(!dist.count(newHash) or d0 + 1 < dist[newHash]){
        dist[newHash] = d0 + 1;
        pq.emplace(dist[newHash], version, to, newHash);
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
  
  cout << persitentDijkstra(state, n, m) << endl;
  return 0;
}
