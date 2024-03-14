#include "game.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
// MY CODE: 1. Save the player's high scores to a text file called highscores.txt in CSV format.
std::string Game::GetPlayerName() const { return playerName; }
void Game::SetPlayerName(std::string name) { playerName = name; }


void Game::SaveScoreToFile() {
  std::ifstream scoresFile;
  scoresFile.open(scoresFilePath);
  std::vector<std::pair<std::string, int>> scores;

  if (scoresFile.is_open()) {
    std::string line;
    while (std::getline(scoresFile, line)) {
      std::istringstream iss(line);
      std::string name;
      std::string divider;
      int score;
      if (iss >> name >> divider >> score) {
        scores.push_back(std::make_pair(name, score));
      }
    }
    scoresFile.close();
  } else {
    std::cerr << "Unable to open file: " << scoresFilePath << std::endl;
    std::ofstream newScoresFile(scoresFilePath);
    if (newScoresFile.is_open()) {
      newScoresFile.close();
      SaveScoreToFile();
    } else {
      std::cerr << "Unable to create file: " << scoresFilePath << std::endl;
    }
    return;
  }

  scores.push_back(std::make_pair(playerName, score));
  std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
    return a.second > b.second;
  });

  std::ofstream updatedScoresFile;
  updatedScoresFile.open(scoresFilePath);
  if (updatedScoresFile.is_open()) {
    int count = 0;
    for (const auto& score : scores) {
      if (count >= 10) {
        break;
      }

      int maxCount = MAX_PLAYER_NAME_LENGTH + 2; 
      std::string divider;
      maxCount -= score.first.length();
      divider = std::string(maxCount, '-');

      updatedScoresFile << score.first << " " << divider << " " << score.second << std::endl;
      count++;
    }
    updatedScoresFile.close();
  } else {
    std::cerr << "Unable to open file: " << scoresFilePath << std::endl;
  }
}

void Game::LoadScoreFromFile() {
  std::ifstream scoresFile;
  scoresFile.open(scoresFilePath);
  if (scoresFile.is_open()) {
    std::cout << "\nSnake Game High Scores!!" << std::endl;
    std::string line;
    while (std::getline(scoresFile, line)) {
      std::cout << line << std::endl;
    }
    scoresFile.close();
  } else {
    std::cerr << "Unable to open file: " << scoresFilePath << std::endl;
  }
}
//
