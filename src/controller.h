#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, std::shared_ptr<Snake> &snake) const;

 private:
  void ChangeDirection(std::shared_ptr<Snake> &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif