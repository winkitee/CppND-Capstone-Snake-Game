#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "snake.h"
#include "obstacle.h"

class Renderer
{
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(
      std::shared_ptr<Snake> const snake,
      std::shared_ptr<Snake> const computerSnake,
      SDL_Point const &food,
      SDL_Point const &special_food,
      std::vector<SDL_Point> const &foods,
      std::vector<Obstacle> const &obstacles,
      std::vector<FixedObstacle> const &fixedObstacles);
  void RenderObstacles(const std::vector<Obstacle> &obstacles);
  void RenderFixedObstacles(const std::vector<FixedObstacle> &fixedObstacles);
  void UpdateWindowTitle(int score, int fps);

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif