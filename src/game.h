#ifndef GAME_H
#define GAME_H

#include <random>
#include <string>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

constexpr std::size_t MAX_PLAYER_NAME_LENGTH{18};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
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

 private:
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  // MY CODE: 1. Save the player's high scores to a text file called highscores.txt in CSV format.
  std::string playerName;
  std::string scoresFilePath{"../highscores.txt"};
  //

  void PlaceFood();
  void Update();
};

#endif