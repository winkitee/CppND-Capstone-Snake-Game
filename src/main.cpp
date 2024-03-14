#include <iostream>
#include <string>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  // MY CODE: 1. Save the player's high scores to a text file called highscores.txt in CSV format.
  std::string name;
  while (name.size() < 1 || name.size() > MAX_PLAYER_NAME_LENGTH) {
    std::cout << "Enter your name: ";
    std::cin >> name;
    
    if (name.size() < 1) {
      std::cout << "Name must be at least 1 character." << std::endl;
    } else if (name.size() > MAX_PLAYER_NAME_LENGTH) {
      std::cout << "Name must be at most " << MAX_PLAYER_NAME_LENGTH << " characters." << std::endl;
    }
  }
  game.SetPlayerName(name);
  game.SaveScoreToFile();
  game.LoadScoreFromFile();
  //
  return 0;
}