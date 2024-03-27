#include "a_star.h"

Node::Node(int x, int y, int g, Node *parent) : x(x), y(y), g(g), parent(parent)
{
  f = g + h;
}

void Node::calculateHeuristic(int endX, int endY)
{
  h = abs(x - endX) + abs(y - endY);
  f = g + h;
}

bool Compare::operator()(const Node *a, const Node *b) const
{
  return a->f > b->f;
}

vector<pair<int, int>> AStar::findPath(vector<vector<int>> &grid, pair<int, int> start, pair<int, int> end)
{
  int rows = grid.size();
  int cols = grid[0].size();
  vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  priority_queue<Node *, vector<Node *>, Compare> openNodes;
  vector<vector<bool>> visited(rows, vector<bool>(cols, false));
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if (grid[i][j] == 1)
      {
        visited[i][j] = true;
      }
    }
  }

  Node *startNode = new Node(start.first, start.second, 0, nullptr);
  startNode->calculateHeuristic(end.first, end.second);
  openNodes.push(startNode);

  while (!openNodes.empty())
  {
    Node *current = openNodes.top();
    openNodes.pop();

    if (current->x == end.first && current->y == end.second)
    {
      vector<pair<int, int>> path;
      while (current != nullptr)
      {
        path.push_back({current->x, current->y});
        current = current->parent;
      }
      reverse(path.begin(), path.end());
      return path;
    }

    visited[current->x][current->y] = true;

    for (auto &dir : directions)
    {
      int newX = current->x + dir.first;
      int newY = current->y + dir.second;

      if (isValid(newX, newY, grid) && !visited[newX][newY])
      {
        Node *neighbor = new Node(newX, newY, current->g + 1, current);
        neighbor->calculateHeuristic(end.first, end.second);
        openNodes.push(neighbor);
        visited[newX][newY] = true;
      }
    }
  }

  return {};
}

bool AStar::isValid(int x, int y, vector<vector<int>> &grid)
{
  int rows = grid.size();
  int cols = grid[0].size();
  return (x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == 0);
}
