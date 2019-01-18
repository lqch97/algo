/*
  LCS problem practicing
  approach: Dynamic programming
  language: C++11
*/

#include <iostream>
#include <vector>
#include <stack>
#include <cstdio>
using namespace std;

enum Direction {W, NW, N}; //west, north-west, north

vector<vector<int> > L; // a table be used in DP
vector<vector<Direction> > previous; //previous position where L[i][j] from

string s1, s2; // input strings

void printTable() {

  cout << "The table be used in LCS:\n\n";
  printf("%4c%4c", '\\', '\\'); // align
  for(char c: s2) {
    printf("%4c", c);
  }
  cout << endl;

  for(int row = 0; row < L.size(); ++row) {
    if(row == 0) printf("%4c", '\\');
    else printf("%4c", s1[row-1]);
    for(int col = 0; col < L[row].size(); ++col) {
      printf("%4d", L[row][col]);
    }
    cout << endl;
  }
  cout << endl;
}

string lcsString() {
  string out_str;
  stack<char> s;
  int m = previous.size() - 1, n = previous[0].size() - 1;
  int len = L[m][n];
  for(int i =  m, j = n; len > 0; ) {
    switch (previous[i][j]) {
      case NW:
        s.push(s1[i - 1]);
        --len;
        --i; --j;
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

string LCS() {
  int m = s1.length(), n = s2.length();

  // tables init
  L.resize(m + 1);
  for(int i = 0; i < L.size(); ++i)
    L[i].resize(n + 1);

  previous.resize(m + 1);
  for(int i = 0; i < L.size(); ++i)
    previous[i].resize(n + 1);

  // DP algo
  for(int i = 0; i <= m; ++i) L[i][0] = 0;
  for(int j = 0; j <= n; ++j) L[0][j] = 0;
  for(int i = 1; i <= m; ++i) {
    for(int j = 1; j <= n; ++j) {
      if(s1[i - 1] == s2[j - 1]) {
        L[i][j] = L[i-1][j-1] + 1;
        previous[i][j] = NW;
      }
      else if(L[i-1][j] > L[i][j-1]) {
        L[i][j] = L[i-1][j];
        previous[i][j] = N;
      }
      else {
        L[i][j] = L[i][j-1];
        previous[i][j] = W;
      }
    }
  }

  printTable();
  //generate the substring
  int len = L[m][n]; //len of LCS
  string out_str = lcsString();

  return out_str;
}

int main() {
  cin >> s1 >> s2;

  cout << "string1: " << s1 << "\n\n";
  cout << "string2: " << s2 << "\n\n";

  string out_str = LCS();
  cout << "LCS: " << out_str << "\n\n";
  cout << "length of LCS: " << out_str.length() << "\n\n";
}
