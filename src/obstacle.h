#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SDL.h"
#include <vector>
#include <random>

class Obstacle {
 public:
  Obstacle(int grid_width, int grid_height, std::pair<int, int> snake_head)
      : grid_width(grid_width),
        grid_height(grid_height),
        dx(1),
        dy(1) 
  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> random_d(0, 1);
    std::uniform_int_distribution<> random_pos(0, grid_width - 1);

    do {
      x = random_pos(gen);
      y = random_pos(gen);
    } while (std::abs(x - snake_head.first) < 8 && std::abs(y - snake_head.second) < 8);

    dx = random_d(gen) == 0 ? -1 : 1;
    dy = random_d(gen) == 0 ? -1 : 1;
  }

  void Update();

  int x;
  int y;

 private:
  int grid_width;
  int grid_height;
  int dx;
  int dy;
};

class FixedObstacle {
 public:
  FixedObstacle(int grid_width, int grid_height, std::pair<int, int> snake_head, std::pair<int, int> food_position)
      : grid_width(grid_width),
        grid_height(grid_height) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> random_pos(0, grid_width - 1);
    std::uniform_int_distribution<> random_dir(0, 3);
    std::uniform_int_distribution<> random_num(1, 3);

    int x, y;
    do {
      x = random_pos(gen);
      y = random_pos(gen);
    } while ((std::abs(x - snake_head.first) < 8 && std::abs(y - snake_head.second) < 8) && 
             (std::abs(x - food_position.first) < 3 && std::abs(y - food_position.second) < 3));
    positions.push_back({x, y});

    for (int i = 1; i < random_num(gen); ++i) {
      do {
        int dir = random_dir(gen);
        switch (dir) {
          case 0: ++x; break;  // right
          case 1: --x; break;  // left
          case 2: ++y; break;  // down
          case 3: --y; break;  // up
        }
        // Ensure the new position is within the grid
        x = std::max(0, std::min(x, grid_width - 1));
        y = std::max(0, std::min(y, grid_height - 1));
      } while ((std::abs(x - snake_head.first) < 8 && std::abs(y - snake_head.second) < 8) && 
               (std::abs(x - food_position.first) < 3 && std::abs(y - food_position.second) < 3));
      positions.push_back({x, y});
    }
  }

  // No need to update the position for a fixed obstacle
  void Update() {}

  std::vector<std::pair<int, int>> positions;

 private:
  int grid_width;
  int grid_height;
};

#endif