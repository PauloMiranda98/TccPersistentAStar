#include <bits/stdc++.h>
 
using namespace std;

const int dx[] = { 0, 0, -1, 1};
const int dy[] = {-1, 1,  0, 0};

void createTestFile(string name, int n, int m, vector<int> v){
  ofstream file(name);
  file << n << " " << m << endl;
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++)
      file << v[i*m + j] << " ";
    file << endl;
  }
  file.close();
}

vector<int> getInitialVector(int n, int m){
  int k = n*m;
  vector<int> v(k, 0);
  iota(v.begin(), v.end(), 1);
  v.back() = 0;
  return v;
}

vector<int> getRandomGameState(int n, int m, int it){
  vector<int> state = getInitialVector(n, m);
  int pos = state.size() - 1;
  int last = -1;
  
  while(it--){
    int i = pos/m, j = pos%m;
    vector<int> options;
    
    for(int k=0; k<4; k++){
      int a = i + dx[k];
      int b = j + dy[k];
      if(a >= 0 and a < n and b >= 0 and b < m){
        int id = a*m + b;
        if(id != last){
          options.push_back(id);
        }
      }
    }
    last = pos;
    int to = options[rand()%options.size()];
    swap(state[pos], state[to]);
    swap(pos, to);
  }
  return state;
}


void createTest(int n, int m){
  for(int it = 10; it <= 100; it += 10){
    for(int t=1; t<=5; t++){
      string sz = to_string(n) + string("x") + to_string(m);
      string iterations = to_string(it);
      string index = to_string(t);
      
      string name = sz + "_" + iterations + "_" + index  + ".txt";
      string path = "./test/";
      vector<int> state = getRandomGameState(n, m, it);
      
      cout << path+name << endl;
      createTestFile(path + name, n, m, state);
    }
  }
}

void createAllTest(){
  for(int n=4; n<10; n++){
    createTest(n, n);
  }
  for(int n=10; n<100; n+=10){
    createTest(n, n);
  }
  for(int n=100; n<=1000; n+=100){
    createTest(n, n);
  }
}

int main() {
  ios_base::sync_with_stdio(false); cin.tie(NULL);
  srand(2021);
  createAllTest();
  return 0;
}
