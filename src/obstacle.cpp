#include "obstacle.h"
#include <iostream>

void Obstacle::Update() {
  x += dx;
  y += dy;

  // If the obstacle hits the edge of the grid, move in a specific direction
  if (x == 0) {
    dx = 1; // move right
  } else if (x == grid_width - 1) {
    dx = -1; // move left
  }

  if (y == 0) {
    dy = 1; // move down
  } else if (y == grid_height - 1) {
    dy = -1; // move up
  }
}