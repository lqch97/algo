/*
  MST & shortest path,
  graph representation: adjacency matrix
*/

#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

//return type for method "mst" in class "Graph"
struct Edge {
  int tail;
  int head;
  double weight;
  Edge(int t, int h, double w): tail(t), head(h), weight(w) {}
};

//return type for "spt" method
struct Path {
  double length;
  vector<int> path;
};

class Graph {
private:
  int num_vert;
  vector<vector<double> > adjMat;
public:
  //constructor
  Graph(): num_vert(0) {};
  Graph(int n): num_vert(n) {
    adjMat.resize(n);
    for(int i = 0; i < n; ++i) {
      adjMat[i].resize(n);
    }
  };
  //methods
  void addEdge(int v1, int v2, double weight);
  void print();
  vector<Edge> mst(int x);
  vector<Path> spt(int x);
};

//method definitions
void Graph::addEdge(int v1, int v2, double weight) {

  v1 -= 1; v2 -= 1; //let v1 and v2 fit the size of vector

  if(v1 < 0 || v1 >= num_vert) {
    cout << "no vertex " << v1 << "." << endl;
    return;
  }

  if(v2 < 0 || v2 >= num_vert) {
    cout << "no vertex " << v2 << "." << endl;
    return;
  }

  if(v1 == v2) {
    cout << "Disallow loop (" << v1 << ", " << v2
      << ")." << endl;
    return;
  }

  if(adjMat[v1][v2])
    cout << "Disallow parallel edge, replace edge("
      << v1 << ", " << v2
      <<") weight " << adjMat[v1][v2] << " to " << weight
      << "." << endl;

  adjMat[v1][v2] = weight;
  adjMat[v2][v1] = weight;
}

void Graph::print() {
  for(int i = 0; i < adjMat.size(); ++i) {
    for(int j = 0; j< adjMat.size(); ++j) {
      if(adjMat[i][j])
        printf("(%d, %d, %.2f)", i, j, adjMat[i][j]);
    }
    cout << endl;
  }
}

vector<Edge> Graph:: mst(int x) {
  //x denote the first vetex be selected
  x -= 1; //fit the range of graph

  vector<Edge> mst_edge;

  int n = adjMat.size();
  vector<int> nearestVert(n);
  vector<double> shortestEdge(n);
  vector<bool> selected(n);

  //step 1 & 2: init
  for(int i = 0; i < n; ++i) {
	selected[i] = false;
    nearestVert[i] = x;
    shortestEdge[i] = -1; //denote -1 as infinite
  }
  selected[x] = true;

  for(int i=0; i < n - 1; ++i) {
    //step 3: modify shortestEdge
    for(int i = 0; i < adjMat[x].size(); ++i) {
      if(!selected[i] && adjMat[x][i] && (adjMat[x][i] < shortestEdge[i] || shortestEdge[i] == -1) ) {
        nearestVert[i] = x;
        shortestEdge[i] = adjMat[x][i];
      }
    }

    // step 4: select shortest edge
    // modify status
    int y = -1; //nearestVert
    for(int i = 0; i < n; ++i) {
      if(!selected[i] && shortestEdge[i] != -1)
      	if(y == -1 || (shortestEdge[i] < shortestEdge[y]) )
        	y = i;
    }
    mst_edge.push_back(Edge(nearestVert[y] + 1, y + 1, shortestEdge[y]));
    selected[y] = true;
    shortestEdge[y] = -1;

    //step 5: check and back to step 3.
    x = y;
  }
  return mst_edge;
}

vector<Path> Graph::spt(int x) {
  //x denote the first vetex be selected
  x -= 1; //fit the range of graph

  int n = adjMat.size();
  vector<Path> ret_val(n);

  vector<double> shortestPath(n);
  vector<bool> selected(n);

  //init
  for(int i = 0; i < n; ++i) {
    selected[i] = false;
    shortestPath[i] = -1; //denote -1 as infinite
  }
  selected[x] = true;

  //modify shortest path info(init)
  for(int i = 0; i < adjMat[x].size(); ++i) {
    if(!selected[i] && adjMat[x][i]) {
        shortestPath[i] = adjMat[x][i];
        ret_val[i].path.push_back(x + 1);
    }
  }

  for(int i=0; i < n - 1; ++i) {

    int y = -1; //nearestVert, denote -1 as haven't assigned
    for(int i = 0; i < n; ++i) {
      if(!selected[i] && shortestPath[i] != -1)
        if(y == -1 || (shortestPath[i] < shortestPath[y]) )
          y = i;
    }
    ret_val[y].length = shortestPath[y];
    selected[y] = true;
    x = y;

    //modify shortest path info(init)
    for(int i = 0; i < adjMat[x].size(); ++i) {
      if(!selected[i] && adjMat[x][i])
        if(shortestPath[i] == -1 ||
            (adjMat[x][i] + shortestPath[x] < shortestPath[i]) ) {
          shortestPath[i] = adjMat[x][i] + shortestPath[x];
          ret_val[i] = ret_val[x];
          ret_val[i].path.push_back(x + 1);
        }
    }
  }
  //last vertex in path of return value
  for(int i = 0; i < ret_val.size(); ++i)
    ret_val[i].path.push_back(i + 1);
  return ret_val;
}

//other functions
Graph graphIn() {
  int n; //num of vertex
  int v1, v2;
  double weight;

  cin >> n;
  Graph g(n);
  while(cin >> v1 >> v2 >> weight) {
    g.addEdge(v1, v2, weight);
  }
  cout << endl;
  return g;
}

int main() {
  Graph g = graphIn();

  //mst
  vector<Edge> mst_edge = g.mst(1);
  cout << "Edges in MST(vertex1, vertex2, weight):\n";
  double sum_weight = 0;
  for(int i = 0; i < mst_edge.size(); ++i) {
    printf("(%d, %d, %.2f)", mst_edge[i].tail, mst_edge[i].head, mst_edge[i].weight);
    sum_weight += mst_edge[i].weight;
    cout << endl;
  }
  printf("Sum of weight: %.2f\n", sum_weight);
  cout << endl;

  //spt
  vector<Path> shortestPath = g.spt(1);
  cout << "Shortest path from vertex1\n";
  for(int i = 1; i < shortestPath.size(); ++i) {
    printf("1 -> %d: %.2f, path:", i + 1, shortestPath[i].length);
    for(int j = 0; j < shortestPath[i].path.size(); ++j)
      cout << shortestPath[i].path[j] << " ";
    cout << endl;
  }
}
