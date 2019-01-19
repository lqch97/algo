/*
	convex hull

	appraoch: divide and conquer

	language: C++11
		because the use of some C++11 feature,
		please compile the code in environments supporting C++11,
		or typing the command "g++ -std=c++11 sourceCode.cpp".

	at line 242
	merging part is not finished
		current version: O(nlog2n)
		target				: O(nlogn)

*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <cstdio>
using namespace std;

struct Point {
	double x;
	double y;
	int ID; // specify by input order

	double polerAngle;

	Point() {};
	Point(double a, double b): x(a), y(b) {};
};

enum sortOption {x, y}; //options that sort algo will sort by

void swap(Point &a, Point &b) {
	Point temp = a;
	a = b;
	b = temp;
}

vector<Point> getInput() {
	FILE *f;
	if (NULL == (f = fopen("test2.txt", "r"))) {
		cout << "Fail to open the file";
		exit(EXIT_FAILURE);
	}

	vector<Point> in;
	Point temp = Point();
	int count = 1;

	while( (fscanf(f, "%lf", &temp.x), fscanf(f, "%lf", &temp.y) ) != EOF) {
		temp.ID = count++;
		in.push_back(temp);
	}
	return in;
}

void printPoint(Point point) {
	printf("%d: (%lf, %lf)\n", point.ID, point.x, point.y);
}

// quickSort
namespace qs {

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
}; // end of namespace qs

//Graham scan
namespace gh {
	enum orientationOption {CW, CCW, CL}; //clockwise, counter CW, collinear
	Point p0; // point with smallest y value

	orientationOption orientation(Point p, Point q, Point r) {
		double val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0.0) return CL;  // colinear
    return (val > 0.0)? CW: CCW; // clock or counterclock wise
	}

	double distanceSquare(Point p1, Point p2) {
    return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
	}

	bool compare(Point p1, Point p2) {
	   //Find orientation
	   int o = orientation(p0, p1, p2);
	   if (o == CL)
	     return (distanceSquare(p0, p2) >= distanceSquare(p0, p1))
			 				? 1 : 0;

	   return (o == CCW)? 1: 0;
	}

	Point nextToTop(stack<Point> s) {
	  Point p = s.top();
	  s.pop();
	  Point ret_val = s.top();
	  s.push(p);
	  return ret_val;
	}

	vector<Point> grahamScan(vector<Point> points, bool needToSort) {
		if(needToSort) {
			// find the point with smallest y value
			p0 = points[0];
			int idx_min = 0;
			for(int i = 1; i < points.size(); ++i) {
				if(points[i].y < p0.y ||
					 (points[i].y == p0.y) && (points[i].x < p0.x)) {
					p0 = points[i];
					idx_min = i;
				}
			}
			swap(points[0], points[idx_min]);

			// sort elements by polar angle
			sort(points.begin() + 1 , points.end() , compare);

			// remove colinear points only remaining the farthest point
			int m = 1;
			for(int i = 1; i < points.size(); ++i) {
	       while (i < points.size()-1 && orientation(p0, points[i],
	                                    points[i+1]) == CL)
	          ++i;

	       points[m] = points[i];
	       ++m;  // Update size of modified array
			}
			while(points.size() > m) points.pop_back();
		} // sorting part ended

		if(points.size() < 3) return points;

		stack<Point> s;
   	s.push(points[0]);
   	s.push(points[1]);
   	s.push(points[2]);

    // Process remaining n-3 points
    for (int i = 3; i < points.size(); ++i) {
       // Keep removing top while the angle formed by
       // points next-to-top, top, and points[i] makes
       // a non-left turn
       while (orientation(nextToTop(s), s.top(), points[i]) != CCW)
          s.pop();
       s.push(points[i]);
    }

		vector<Point> ret_val;
    // Now stack has the output points, print contents of stack
    while (!s.empty()) {
        Point p = s.top();
        ret_val.push_back(p);
        s.pop();
    }

		// reverse the vector
		reverse(ret_val.begin(), ret_val.end());

		return ret_val;
	}

}; // end of gh

//convex hull
namespace cv {

	vector<Point> twoWayMerge(Point base,
				vector<Point> list1, vector<Point> list2) {
		int i, j;
		gh::p0 = base;
		vector<Point> ret_val;
		for(i = 0, j = 0; i < (list1.size()) && (j < list2.size()); ) {
			if(gh::compare(list1[i], list2[j]) == 1) {
				ret_val.push_back(list1[i++]);
			}
			else {
				ret_val.push_back(list2[j++]);
			}
		}

		while(i < list1.size()) ret_val.push_back(list1[i++]);
		while(j < list2.size()) ret_val.push_back(list2[j++]);

		return ret_val;
	}

#if 1
	vector<Point> merge(vector<Point> left, vector<Point> right) {
		vector<Point> ret_val;
		for(auto p: right) left.push_back(p);
		ret_val = gh::grahamScan(left, true);

		return ret_val;
	}
#else
	vector<Point> merge(vector<Point> left, vector<Point> right) {
		Point interior;
		for(auto p: left) {
			interior.x += p.x;
			interior.y += p.y;
		}
		interior.x /= left.size();
		interior.y /= left.size();

		// find the position to split 'right' list
		int splitPoint = 0;
		for(int i = 0; i < right.size() - 1; ++i) {
			if(gh::orientation(interior, right[i], right[i+1]) == gh::CW) {
				splitPoint = i + 1; break;
			}
		}

		// split right into two part
		// so that polar angle increase individually
		vector<Point> r1(right.begin(), right.begin() + splitPoint);
	  vector<Point> r2(right.begin() + splitPoint , right.end());
		reverse(r2.begin(), r2.end());

		right = twoWayMerge(interior, r1, r2);
		left = twoWayMerge(interior, left, right);
		left = gh::grahamScan(left, false);

		// return vector<Point>();
		return left;
	}
#endif

	vector<Point> convexHull(vector<Point> input, bool firstItr) {
		// firstItr := first iteration

		if(input.size() <= 5) {
			vector<Point> ret_val = gh::grahamScan(input, true);
			return ret_val;
		}
	  qs::quickSort(input, x);
	  //split the input array
	  size_t const half_size = input.size() / 2;
	  vector<Point> left(input.begin(), input.begin() + half_size); //left part of input
	  vector<Point> right(input.begin() + half_size, input.end()); //right part

	  left = convexHull(left, false);
	  right = convexHull(right, false);

		// output left and right in last iteration for test
		if(firstItr) {
			cout << "In last iteration:\n\n";
			cout << "left hull:\n\n";
			for(auto x: left) {
				printPoint(x);
			}
			cout << endl;
			cout << "right hull:\n\n";
			for(auto x: right) {
				printPoint(x);
			}
			cout << endl;
		}

		vector<Point> ret_val = merge(left, right);
		return ret_val;
	}
}; // end of cv

int main() {
	vector<Point> list, temp;
	list = getInput();
	cout << "inputted points:\n\n";
	for(auto x: list) {
		printPoint(x);
	}
	cout << endl;

	// temp = gh:: grahamScan(list, true);
	// cout << "g scan:\n\n";
	// for(auto x: temp) {
	// 	printPoint(x);
	// }
	// cout << endl;

	temp = cv:: convexHull(list, true);
	cout << "The convex hull:\n\n";
	for(auto x: temp) {
		printPoint(x);
	}
	cout << endl;
}
