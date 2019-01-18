/*
  selection problem: find k smallest num in a given list

  approach: prune and search

  language: C++11

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <limits> // for max double
using namespace std;

vector<double> getInput() {
  FILE *f;
	if (NULL == (f = fopen("src.txt", "r"))) {
		cout << "Fail to open the file";
		exit(EXIT_FAILURE);
	}

  vector<double> ret_val;
  double temp;
  while(fscanf(f, "%lf", &temp) != EOF) {
    ret_val.push_back(temp);
  }
  return ret_val;
}

double smallest(vector<double> in_list, int key) {

  // if fewer than 5 num, solve brutally
  if(in_list.size() <= 5) {
    sort(in_list.begin(), in_list.end());
    return in_list[key-1];
  }

  // split list into ceil(size / 5) lists
  int list_num = ceil( (double) in_list.size() / 5 );
  vector<vector<double> > lists(list_num);

  int currnt_list = 0;
  for(auto x: in_list) {
    lists[currnt_list].push_back(x);
    if(lists[currnt_list].size() == 5)
      ++currnt_list;
  }
  while(lists[currnt_list].size() < 5)
    lists[currnt_list].push_back(numeric_limits<double>::max()); // max double

  // sort lists individually
  for(int i = 0; i < lists.size(); ++i) {
    sort(lists[i].begin(), lists[i].end());
  }

  // make a list for all middle num in lists
  // and find the middle num in list recursively
  vector<double> mid_list;
  for(auto list: lists) {
    mid_list.push_back(list[2]);
  }
  double mid_num = smallest(mid_list, (mid_list.size() + 1) / 2);

  // split original list into list1 ,list2, list3
  // by the relation between mid_num
  vector<double> list1, list2, list3;
  for(auto x: in_list) {
    if(x < mid_num)
      list1.push_back(x);
    else if(x == mid_num)
      list2.push_back(x);
    else
      list3.push_back(x);
  }

  // prune the not needed lists,
  // and solve the problem recursively
  if(list1.size() >= key) {
    return smallest(list1, key);
  }
  else if(list1.size() + list2.size() >= key) {
    return list2[0];
  }
  else {
    int smallerThan = list1.size() + list2.size();
    return smallest(list3, key - smallerThan);
  }
}

int main() {
  vector<double> v = getInput();
  cout << "nums from file was read.\n";

  int k;
  cout << "\nplease input k, so that k-th num will be searched, ";
  cout << "(non-integer to terminate): ";
  while(cin >> k) {
    if(k < 1 || k > v.size()) {
      cout << "\nout of range.\n";
    }
    else {
      double sel_num = smallest(v, k);
      cout << endl << k << "-th smallest num is ";
      cout << sel_num << endl;
    }
    cout << "\nplease input k, so that k-th num will be searched: ";
  }
  cout << "\ndone.\n";
}
