#include "route_model.h"

RouteModel::RouteModel() {

}

// void RouteModel::MoveGameHere(std::unique_ptr<Game> game)
// {
//   _game = std::move(game);
// }

// void RouteModel::MoveNext(Game &game)
// {
//   auto computerSnake = game.GetComputerSnake();
//   auto food = game.GetFood();

//   AStar astar;
//   auto grid = game.GetGrid();
//   auto path = astar.findPath(grid, {computerSnake->head_x, computerSnake->head_y}, {food.x, food.y});

//   if (path.empty())
//   {
//     return;
//   }

//   auto nextPosition = path.front();
//   int next_x = nextPosition.first;
//   int next_y = nextPosition.second;
//   int x = computerSnake->head_x;
//   int y = computerSnake->head_y;

//   if (next_x < x)
//   {
//     computerSnake->direction = Snake::Direction::kLeft;
//   }
//   else if (next_x > x)
//   {
//     computerSnake->direction = Snake::Direction::kRight;
//   }
//   else if (next_y < y)
//   {
//     computerSnake->direction = Snake::Direction::kUp;
//   }
//   else if (next_y > y)
//   {
//     computerSnake->direction = Snake::Direction::kDown;
//   }
// }
