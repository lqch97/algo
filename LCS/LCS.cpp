/*
  LCS problem practicing
  approach: Dynamic programming
*/

#include <iostream>
#include <vector>
#include <stack>
using namespace std;

string s1, s2;

enum direction {W, NW, N}; //west, north-west, north

string LCS(string s1, string s2) {
  int m = s1.length(), n = s2.length();
  int L[m+1][n+1];
  direction prev[m+1][n+1]; //previous position where L[i][j] from
  stack<char> s;

  for(int i = 0; i <= m; ++i) L[i][0] = 0;
  for(int j = 0; j <= n; ++j) L[0][j] = 0;
  for(int i = 1; i <= m; ++i) {
    for(int j = 1; j <= n; ++j) {
      if(s1[i] == s2[j]) {
        L[i][j] = L[i-1][j-1] + 1;
        prev[i][j] = NW;
      }
      else if(L[i-1][j] > L[i][j-1]) {
        L[i][j] = L[i-1][j];
        prev[i][j] = N;
      }
      else {
        L[i][j] = L[i][j-1];
        prev[i][j] = W;
      }
    }
  }
  //return L[m][n];
  //LCS is now in L[m][n]
  //generate the substring
  int len = L[m][n]; //len of LCS
  string out_str;
  for(int i = m , j = n ; len > 0; ) {
    switch (prev[i][j]) {
      case NW:
        --len;
        --i; --j;
        s.push(s1[i]);
        break;
      case W : --j; break;
      case N : --i;
    }
  }
  while(!s.empty()) {
    out_str += s.top(); s.pop();
  }
  return out_str;
}

int main() {
  string s1, s2;
  cin >> s1 >> s2;
  cout << LCS(s1, s2);
}
