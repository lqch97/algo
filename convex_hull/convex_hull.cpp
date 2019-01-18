/*
	convex hull

	appraoch: divide and conquer

	language: c++11
*/


#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

struct Point {
	double x;
	double y;

	double polerAngle;

	Point(): {};
	Point(double a, double b): x(a), y(b) {};
};

enum sortOption {x, y, angle}; //options that sort algo will sort by

//sorting part
void swap(Point &a, Point &b) {
	Point temp = a;
	a = b;
	b = temp;
}

//the recursive subroutine for quickSort
void quickSortRec(vector<Point> &v, int start, int end, sortOption op) {
	if(start >= end)
		return;

	int i = start + 1, j = end;
	int pivot = start;

	//sort algo different from op
	if(op == x) {
		while(i <= j) {
			while( (v[i].x <= v[pivot].x) && (i <= end) )
				++i;
			while( (v[j].x >= v[pivot].x) && (j >= start + 1) )
				--j;
			if(i < j)
				swap(v[i], v[j]);
		}
	}
	else if(op == y) {
		while(i <= j) {
			while( (v[i].y <= v[pivot].y) && (i <= end) )
				++i;
			while( (v[j].y >= v[pivot].y) && (j >= start + 1) )
				--j;
			if(i < j)
				swap(v[i], v[j]);
		}
	}

	swap(v[j], v[start]);
	quickSortRec(v, start, j - 1, op);
	quickSortRec(v, j + 1, end, op);
}

void quickSort(vector<Point> &v, sortOption op) {
	int start = 0;
	int end = v.size() - 1;
	quickSortRec(v, start, end, op);
}
//sort part end

vector<Point> getInput() {
	vector<Point> in;
	while(!cin.eof()) {
		Point temp = Point();
		cin >> temp.x;
		cin >> temp.y;
		in.push_back(temp);
	}
	return in;
}

//conve hull funcs
Point mid;
int quad(pair<int, int> p)
{
    if (p.first >= 0 && p.second >= 0)
        return 1;
    if (p.first <= 0 && p.second >= 0)
        return 2;
    if (p.first <= 0 && p.second <= 0)
        return 3;
    return 4;
}

bool compare(Point p1, Point q1)
{
    pair<double, double> p = make_pair(p1.x - mid.x,
                                 p1.y - mid.y);
    pair<double, double> q = make_pair(q1.x - mid.x,
                                 q1.y - mid.y);

    int one = quad(p);
    int two = quad(q);

    if (one != two)
        return (one < two);
    return (p.second*q.first < q.second*p.first);
}

vector<Point> bruteHull(vector<Point> v) {
	vector<Point> hull;

	//take any two point, if all other point is of same side of the edge
	//of two point, the egde if in convexHUll
	for(int i = 0; i < v.size(); ++i) {
		for(int j = i + 1; j < v.size(); ++j) {
			int x1 = v[i].x, x2 = v[j].x;
			int y1 = v[i].y, y2 = v[j].y;

			int a1 = y1-y2;
			int b1 = x2-x1;
			int c1 = x1*y2-y1*x2;
			int pos = 0, neg = 0;
			for (int k=0; k<v.size(); k++)
			{
					if (a1*v[k].x+b1*v[k].y+c1 <= 0)
							neg++;
					if (a1*v[k].x+b1*v[k].y+c1 >= 0)
							pos++;
			}
			if (pos == v.size() || neg == v.size())
			{
					hull.push_back(v[i]);
					hull.push_back(v[j]);
			}
		}
	}

	// Sorting the points in the anti-clockwise order
	mid = Point(0,0);
	int n = ret.size();
	for (int i=0; i<n; i++)
	{
			mid.x += ret[i].x;
			mid.y += ret[i].y;
			ret[i].x *= n;
			ret[i].y *= n;
	}
	sort(ret.begin(), ret.end(), compare);
	for (int i=0; i<n; i++)
			ret[i] = make_pair(ret[i].first/n, ret[i].second/n);

	return hull;
}

vector<Point> merge(vector<Point> left, vector<Point> right) {
	vector<Point> ret_val;

	return ret_val;
}

vector<Point> convexHull(vector<Point> input) {
	if(input.size() <= 5) {
		vector<Point> ret_val = bruteHull(input);
		return ret_val;
	}
  quickSort(input, x);
  //split the input array
  size_t const half_size = input.size() / 2;
  vector<Point> left(input.begin(), input.begin() + half_size); //left part of input
  vector<Point> right(input.begin() + half_size, input.end()); //right part

  left = convexHull(left);
  right = convexHull(right);

	vector<Point> ret_val = merge(left, right);
	return ret_val;
}

int main() {
	vector<Point> v;
	v = getInput();
	quickSort(v, y);
	for(auto x: v) {
		cout << "(" << x.x << " " << x.y << ")" << endl;
	}
}
