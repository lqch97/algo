/*
  8 - puzzle problem
  approach: tree search(hill climbing)

  because this program is writing with some
  "c++11 feature"

  please compile it with compiler supporting C++11,
  or run the code in:
    https://ideone.com/
*/

#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
using namespace std;

enum Direction {U, D, L, R};

typedef vector<int> Puzzle ;

Puzzle goal = {1, 2, 3, 8, 0, 4, 7, 6, 5}; // goal node for the game

set<Puzzle> selected; // puzzles that have been used

struct Step {
  int grid;
  Direction direction;

  Step(){};
  Step(int g, Direction dir): grid(g), direction(dir) {};
};

struct Node {
  Puzzle puzzle;
  vector<Step> steps;

  Node(){};
  Node(Puzzle p): puzzle(p) {};
};

string strDir(Direction d) {
	switch (d) {
		case U:
			return "up";
		case D:
			return "down";
		case L:
			return "left";
		case R:
			return "right";
	}
}

void getInput(vector<int> &puzzle) {
  int temp;
  for(int i = 0; i < 9; ++i) {
    cin >> temp;
    puzzle.push_back(temp);
  }
}

int evaluate(vector<int> puzzle) {
  vector<int> goalPosition(9);
  vector<int> puzzlePosition(9);
  int score = 0;

  for(int i = 0; i < 9; ++i) {
    goalPosition[goal[i]] = i;
    puzzlePosition[puzzle[i]] = i;
  }
  for(int i = 1; i < 9; ++i) {
    if(goalPosition[i] != puzzlePosition[i])
      ++score;
  }
  return score;
}

void swap(int &a, int &b) {
	int temp = a;
	a = b;
	b = temp;
}

Node move(Node root, int from, int to, Direction d) {
	Puzzle temp_puzzle;
	Step temp_step;
	Node temp_node;

	temp_puzzle = root.puzzle;
	temp_step.grid = temp_puzzle[from];
	temp_step.direction = d;
	swap(temp_puzzle[from], temp_puzzle[to]);
	temp_node.puzzle = temp_puzzle;
	temp_node.steps = root.steps;
	temp_node.steps.push_back(temp_step);

	return temp_node;
}

vector<Node> getBranches(Node root) {
  vector<Node> ret_val;
  int space;
  Node temp_node;
  Puzzle temp_puzzle;
  Step temp_step;

  // find space Position
  for(int i = 0; i < 9; ++i) {
    if(root.puzzle[i] == 0)
      space = i;
  }

  // get branches
  if(space ==  0) {
		// move up
		temp_node = move(root, 3, 0, U);
		ret_val.push_back(temp_node);
		// move left
		temp_node = move(root, 1, 0, L);
		ret_val.push_back(temp_node);
  }

	else if(space ==  1) {
    // move to right
		temp_node = move(root, 0, 1, R);
		ret_val.push_back(temp_node);
    // move up
		temp_node = move(root, 4, 1, U);
		ret_val.push_back(temp_node);
		// move left
		temp_node = move(root, 2, 1, L);
		ret_val.push_back(temp_node);
  }

	else if(space ==  2) {
    // move to right
		temp_node = move(root, 1, 2, R);
		ret_val.push_back(temp_node);
    // move up
		temp_node = move(root, 5, 2, U);
		ret_val.push_back(temp_node);
  }

	else if(space ==  3) {
    // move down
		temp_node = move(root, 0, 3, D);
		ret_val.push_back(temp_node);
    // move left
		temp_node = move(root, 4, 3, L);
		ret_val.push_back(temp_node);
		// move up
		temp_node = move(root, 6, 3, U);
		ret_val.push_back(temp_node);
  }

	else if(space ==  4) {
    // move down
		temp_node = move(root, 1, 4, D);
		ret_val.push_back(temp_node);
    // move right
		temp_node = move(root, 3, 4, R);
		ret_val.push_back(temp_node);
		// move left
		temp_node = move(root, 5, 4, L);
		ret_val.push_back(temp_node);
		// move up
		temp_node = move(root, 7, 4, U);
		ret_val.push_back(temp_node);
  }

	else if(space ==  5) {
    // move down
		temp_node = move(root, 2, 5, D);
		ret_val.push_back(temp_node);
    // move right
		temp_node = move(root, 4, 5, R);
		ret_val.push_back(temp_node);
		// move up
		temp_node = move(root, 8, 5, U);
		ret_val.push_back(temp_node);
  }

	else if(space ==  6) {
    // move down
		temp_node = move(root, 3, 6, D);
		ret_val.push_back(temp_node);
    // move right
		temp_node = move(root, 7, 6, L);
		ret_val.push_back(temp_node);
  }

	else if(space ==  7) {
    // move down
		temp_node = move(root, 4, 7, D);
		ret_val.push_back(temp_node);
    // move right
		temp_node = move(root, 6, 7, R);
		ret_val.push_back(temp_node);
		// move left
		temp_node = move(root, 8, 7, L);
		ret_val.push_back(temp_node);
  }

	else if(space ==  8) {
    // move down
		temp_node = move(root, 5, 8, D);
		ret_val.push_back(temp_node);
    // move right
		temp_node = move(root, 7, 8, R);
		ret_val.push_back(temp_node);
  }

  return ret_val;
}

// a func be used for sorting
bool compare(Node a, Node b) {
  return ( evaluate(a.puzzle) > evaluate(b.puzzle) );
}

bool isSelected(Puzzle puzzle) {
  // check current node is selected or not
  bool ret_val = 0;
  for(auto p: selected) {
    if(p == puzzle) {
      ret_val = 1;
      break;
    }
  }
  return ret_val;
}

Node gamePlaying(vector<int> puzzle) {
  //init first state
  stack<Node> s; // a stack used for hill climbing algo
  Node root(puzzle);
  selected.insert(root.puzzle);
  s.push(root);

  // start hill climbing algo
  while(!s.empty()) {
    root = s.top(); s.pop();

    if(root.puzzle == goal)
      return root;

    vector<Node> branches = getBranches(root);

    sort(branches.begin(), branches.end(), compare);
    for(auto b: branches) {
      if(!isSelected(b.puzzle)) {
        selected.insert(b.puzzle);
        s.push(b);
      }
    }
  }

  cout << "search failure\n";
  return Node();
}

void branchesInfo(Node root) {
  vector<Node> nodes = getBranches(root);
  sort(nodes.begin(), nodes.end(), compare);
  for(auto node: nodes) {
    cout << "puzzle: ";
    for(auto x: node.puzzle)
    cout << x;
    cout << endl;
    cout << "steps: ";
    for(auto p: node.steps) {
      cout << p.grid << " " << strDir(p.direction);
      cout << endl;
    }
    cout << endl;
  }
}

void printSteps(Node node) {
  int stepCount = 0;
	cout << "goal: ";
    for(auto x: node.puzzle)
    cout << x;
    cout << endl;
    cout << "steps: \n";
    for(auto p: node.steps) {
      cout << p.grid << " " << strDir(p.direction);
      cout << endl;
      ++stepCount;
    }
    cout << "total " << stepCount << " steps.";
    cout << endl;
}

int main() {
  vector<int> puzzle;

  cout << "Please input the init puzzle(ex: 2 0 3 1 8 4 7 6 5):\n";
  getInput(puzzle);
  Node lastNode = gamePlaying(puzzle);

  cout << "init: ";
  for(int x : puzzle)
	cout << x;
  cout << endl;

  printSteps(lastNode);
}
