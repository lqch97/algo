/*
  please compile the code with C++11 compiler
	2D Ranking
	without file I/O
*/

#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;

struct Point {
  float x;
  float y;
  int rank;

  int order; //the original ordering of every point
  bool leftSide; //to distinct two splited arr after join
};

enum sortOption {x, y, order}; //options that sort algo will sort by

vector<Point> getInput() {
  int count = 0;
  vector<Point> input;
  Point temp = Point();
  while(scanf("%f%f", &temp.x, &temp.y) != EOF ) {
    temp.order = count++;
    input.push_back(temp);
  }
  return input;
}

void printP(Point p) {
  printf("(%.2f, %.2f): %d\n", p.x, p.y, p.rank);
}

void output(vector<Point> input) {
	int pointNum = input.size(),
		maxRank = input[0].rank,
		minRank = input[0].rank,
		sum = 0;
	for(auto x: input) {
		sum += x.rank;
		if(x.rank > maxRank)
			maxRank = x.rank;
		if(x.rank < minRank)
			minRank = x.rank;
		printP(x);
	}
	cout << "num of point: " << pointNum << endl
		<< "maxRank: " << maxRank << endl
		<< "minRank: " << minRank << endl
		<< "aveRank: " << (float)sum / pointNum << endl;
}

void swap(Point &a, Point &b) {
	Point temp = a;
	a = b;
	b = temp;
}

void restore(vector<Point> &list, int root, int last, sortOption op) {
  int left = root * 2 + 1,
    right = root * 2 + 2,
    largest = root;

  //sort by x
  if(op == x) {
	  if(left <= last && list[left].x > list[largest].x) {
	    largest = left;
	  }
	  if(right <= last && list[right].x > list[largest].x) {
	    largest = right;
	  }
  }

  //sort by y
  else if(op == y) {
	  if(left <= last && list[left].y > list[largest].y) {
	    largest = left;
	  }
	  if(right <= last && list[right].y > list[largest].y) {
	    largest = right;
	  }
  }

  //sort by original order
  else if(op == order) {
	  if(left <= last && list[left].order > list[largest].order) {
	    largest = left;
	  }
	  if(right <= last && list[right].order > list[largest].order) {
	    largest = right;
	  }
  }

  if(largest != root) {
    swap(list[largest], list[root]);
    restore(list, largest, last, op);
  }
}

void construct(vector<Point> &list, sortOption op) {
  //construct a max heap for heapsort
  int last = list.size() - 1;
  for(int i = (last - 1) / 2; i >= 0; --i) {
    restore(list, i, last, op);
  }
}

void heapsort(vector<Point> &list, sortOption op) {
  int last = list.size() - 1;
  construct(list, op);
  while(last > 0) {
    swap(list[0], list[last]);
    last -= 1;
    restore(list, 0, last, op);
  }
}

vector<Point> ranking(vector<Point> input) {
  if(input.size() == 1) {
    input[0].rank = 0;
    return input;
  }
	heapsort(input, x);

  //split the input array
  size_t const half_size = input.size() / 2;
  vector<Point> left(input.begin(), input.begin() + half_size); //left part of input
  vector<Point> right(input.begin() + half_size, input.end()); //right part

  left = ranking(left);
  right = ranking(right);

  input.clear();
  for(auto x: left) {
    x.leftSide = true;
    input.push_back(x);
  }
  for(auto x: right) {
    x.leftSide = false;
    input.push_back(x);
  }
  heapsort(input, y);

  for(int i = 0, count = 0; i < input.size(); ++i) {
    if(input[i].leftSide == true)
      ++count;
    else {
      input[i].rank += count;
    }
  }

  return input;
}

int main() {
  vector<Point> input;
  input = getInput();
  input = ranking(input);
  heapsort(input, order);	//reordering the output
  output(input);
}
