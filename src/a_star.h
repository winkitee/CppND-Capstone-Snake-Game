#ifndef A_STAR_H
#define A_STAR_H

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

class Node
{
public:
  int x, y;     // Coordinates of the cell
  int f, g, h;  // Cost values for A* algorithm
  Node *parent; // Pointer to parent cell

  Node(int x, int y, int g, Node *parent);
  void calculateHeuristic(int endX, int endY);
};

struct Compare
{
  bool operator()(const Node *a, const Node *b) const;
};

class AStar
{
public:
  static vector<pair<int, int>> findPath(vector<vector<int>> &grid, pair<int, int> start, pair<int, int> end);

private:
  static bool isValid(int x, int y, vector<vector<int>> &grid);
};

#endif // A_STAR_H
