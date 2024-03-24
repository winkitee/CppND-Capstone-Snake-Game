#ifndef GAME_H
#define GAME_H

#include <random>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "obstacle.h"

constexpr std::size_t MAX_PLAYER_NAME_LENGTH{18};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  // MY CODE: 1. Save the player's high scores to a text file called highscores.txt in CSV format.
  std::string GetPlayerName() const;
  void SetPlayerName(std::string name);
  void SaveScoreToFile();
  void LoadScoreFromFile();
  //
  void SpecialFoodThread();
  void SpecialFoodMoveThread();

 private:
  Snake snake;
  SDL_Point food;
  SDL_Point special_food;
  bool special_food_exists = false;
  std::vector<SDL_Point> foods;
  std::vector<Obstacle> obstacles;
  std::vector<FixedObstacle> fixedObstacles;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  int grid_width;
  int grid_height;

  int score{0};
  int lastObstacleScore{0};
  int lastFixedObstacleScore{0};
  // MY CODE: 1. Save the player's high scores to a text file called highscores.txt in CSV format.
  std::string playerName;
  std::string scoresFilePath{"../highscores.txt"};
  std::thread special_food_thread;
  std::thread special_food_move_thread;
  std::mutex mtx;
  std::condition_variable cv;
  bool game_over = false;
  //

  void PlaceFood();
  void Update();
};

#endif