#include "game.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <future>
#include <thread>
#include <algorithm>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : grid_width(grid_width),
      grid_height(grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1))
{
  std::cout << "Game Constructor called" << std::endl;

  snake = std::make_unique<Snake>(grid_width, grid_height);
  computerSnake = std::make_unique<Snake>(grid_width, grid_height);

  special_food = {-1, -1};
  special_food_exists = false;

  PlaceFood();
  obstacles_thread = std::thread([this, grid_width, grid_height]() {
    while (!game_over) {
      for (auto &obstacle : obstacles) {
        obstacle.Update();
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } });
  special_food_thread = std::thread(&Game::SpecialFoodThread, this);
  special_food_move_thread = std::thread(&Game::SpecialFoodMoveThread, this);
}

Game::~Game()
{
  std::cout << "Game Destructor called" << std::endl;

  game_over = true;
  if (obstacles_thread.joinable())
  {
    obstacles_thread.join();
  }
  if (special_food_thread.joinable())
  {
    special_food_thread.join();
  }
  if (special_food_move_thread.joinable())
  {
    special_food_move_thread.join();
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, computerSnake, food, special_food, foods, obstacles, fixedObstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y))
    {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update()
{
  if (!snake->alive)
  {
    game_over = true;
    return;
  }
  if (!computerSnake->alive)
  {
    computerSnake->body.clear();
    computerSnake->alive = true;
    computerSnake->speed = 0.1f;
  }
  UpdateComputerSnake();

  snake->Update();
  computerSnake->Update();

  int new_x = static_cast<int>(snake->head_x);
  int new_y = static_cast<int>(snake->head_y);
  int computer_new_x = static_cast<int>(computerSnake->head_x);
  int computer_new_y = static_cast<int>(computerSnake->head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y)
  {
    score++;
    PlaceFood();
    snake->GrowBody();
    snake->speed += 0.02;
  }

  if (food.x == computer_new_x && food.y == computer_new_y)
  {
    computerScore++;
    PlaceFood();
    computerSnake->GrowBody();
    computerSnake->speed += 0.01;
  }

  for (auto it = foods.begin(); it != foods.end(); ++it)
  {
    if (it->x == new_x && it->y == new_y)
    {
      foods.erase(it);
      score++;
      lastFixedObstacleScore = score;
      snake->GrowBody();
      snake->speed += 0.002;
      break;
    }
  }

  for (auto it = foods.begin(); it != foods.end(); ++it)
  {
    if (it->x == computer_new_x && it->y == computer_new_y)
    {
      foods.erase(it);
      computerScore++;
      computerSnake->GrowBody();
      computerSnake->speed += 0.001;
      break;
    }
  }

  for (const auto &obstacle : obstacles)
  {
    if (obstacle.x == new_x && obstacle.y == new_y)
    {
      snake->alive = false;
      return;
    }

    if (obstacle.x == computer_new_x && obstacle.y == computer_new_y)
    {
      computerSnake->alive = false;
      return;
    }
  }

  for (const auto &point : computerSnake->body)
  {
    if (point.x == new_x && point.y == new_y)
    {
      snake->alive = false;
      return;
    }
  }

  for (const auto &point : snake->body)
  {
    if (point.x == computer_new_x && point.y == computer_new_y)
    {
      computerSnake->alive = false;
      return;
    }
  }

  // Check if the snake has collided with a fixed obstacle
  for (const FixedObstacle &obstacle : fixedObstacles)
  {
    for (const auto &position : obstacle.positions)
    {
      if (position.first == new_x && position.second == new_y)
      {
        // The snake has collided with a fixed obstacle, end the game
        snake->alive = false;
        return;
      }

      if (position.first == computer_new_x && position.second == computer_new_y)
      {
        // The computer snake has collided with a fixed obstacle, end the game
        computerSnake->alive = false;
        return;
      }
    }
  }

  std::pair<int, int> head = std::pair(new_x, new_y);
  std::pair<int, int> food_position = std::pair(food.x, food.y);

  // Check if the score has increased and we haven't added a fixed obstacle for this score yet
  if (score != lastFixedObstacleScore || computerScore != lastFixedObstacleComputerScore)
  {
    // Add a new fixed obstacle
    fixedObstacles.emplace_back(
        FixedObstacle(grid_width, grid_height, head, food_position));

    // Update the last score we added a fixed obstacle at
    lastFixedObstacleScore = score;
    lastFixedObstacleComputerScore = computerScore;
  }

  if (score % 5 == 0 && score != lastObstacleScore)
  {
    obstacles.emplace_back(
        Obstacle(grid_width, grid_height, head));

    lastObstacleScore = score;
  }

  std::unique_lock<std::mutex> lck(mtx);
  if (special_food_exists && special_food.x == new_x && special_food.y == new_y)
  {
    special_food_exists = false;
    special_food = {-1, -1};

    // Change all obstacles to food
    for (auto &obstacle : fixedObstacles)
    {
      for (const auto &position : obstacle.positions)
      {
        foods.emplace_back(SDL_Point{position.first, position.second});
      }
    }
    fixedObstacles.clear();

    for (auto &obstacle : obstacles)
    {
      foods.emplace_back(SDL_Point{obstacle.x, obstacle.y});
    }
    obstacles.clear();
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake->size; }
// MY CODE: 1. Save the player's high scores to a text file called highscores.txt in CSV format.
std::string Game::GetPlayerName() const { return playerName; }
void Game::SetPlayerName(std::string name) { playerName = name; }

void Game::SaveScoreToFile()
{
  std::ifstream scoresFile;
  scoresFile.open(scoresFilePath);
  std::vector<std::pair<std::string, int>> scores;

  if (scoresFile.is_open())
  {
    std::string line;
    while (std::getline(scoresFile, line))
    {
      std::istringstream iss(line);
      std::string name;
      std::string divider;
      int score;
      if (iss >> name >> divider >> score)
      {
        scores.push_back(std::make_pair(name, score));
      }
    }
    scoresFile.close();
  }
  else
  {
    std::cerr << "Unable to open file: " << scoresFilePath << std::endl;
    std::ofstream newScoresFile(scoresFilePath);
    if (newScoresFile.is_open())
    {
      newScoresFile.close();
      SaveScoreToFile();
    }
    else
    {
      std::cerr << "Unable to create file: " << scoresFilePath << std::endl;
    }
    return;
  }

  scores.push_back(std::make_pair(playerName, score));
  std::sort(scores.begin(), scores.end(), [](const auto &a, const auto &b)
            { return a.second > b.second; });

  std::ofstream updatedScoresFile;
  updatedScoresFile.open(scoresFilePath);
  if (updatedScoresFile.is_open())
  {
    int count = 0;
    for (const auto &score : scores)
    {
      if (count >= 10)
      {
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
  }
  else
  {
    std::cerr << "Unable to open file: " << scoresFilePath << std::endl;
  }
}

void Game::LoadScoreFromFile()
{
  std::ifstream scoresFile;
  scoresFile.open(scoresFilePath);
  if (scoresFile.is_open())
  {
    std::cout << "\nSnake Game High Scores!!" << std::endl;
    std::string line;
    while (std::getline(scoresFile, line))
    {
      std::cout << line << std::endl;
    }
    scoresFile.close();
  }
  else
  {
    std::cerr << "Unable to open file: " << scoresFilePath << std::endl;
  }
}
//

void Game::SpecialFoodThread()
{
  while (!game_over)
  {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::unique_lock<std::mutex> lck(mtx);
    if (!special_food_exists)
    {
      int x = random_w(engine);
      int y = random_h(engine);
      if (x == food.x && y == food.y)
      {
        continue;
      }
      special_food = {random_w(engine), random_h(engine)};
      special_food_exists = true;
      cv.notify_one();
    }
  }
}

void Game::SpecialFoodMoveThread()
{
  std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

  while (!game_over)
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, [this]
            { return special_food_exists; });
    if (special_food_exists)
    {
      std::uniform_int_distribution<int> random_direction(0, directions.size() - 1);
      auto direction = directions[random_direction(engine)];
      special_food.x += direction.first;
      special_food.y += direction.second;

      // Ensure the special food stays within the game boundaries
      special_food.x = std::max(0, std::min(special_food.x, grid_width - 1));
      special_food.y = std::max(0, std::min(special_food.y, grid_height - 1));
    }
  }
}

void Game::UpdateComputerSnake() {
  auto computerSnake = GetComputerSnake();
  auto food = GetFood();

  AStar astar;
  auto grid = GetGrid();
  auto path = astar.findPath(grid, {computerSnake->head_x, computerSnake->head_y}, {food.x, food.y});

  if (path.empty())
  {
    std::cout << "Computer Snake Path: No path found" << std::endl;
    return;
  }

  auto nextPosition = path.size() <= 2 ? path.back() : path[path.size() - 2];
  int next_x = nextPosition.first;
  int next_y = nextPosition.second;
  int x = computerSnake->head_x;
  int y = computerSnake->head_y;
  if (next_x < x)
  {
    computerSnake->direction = Snake::Direction::kLeft;
  }
  else if (next_x > x)
  {
    computerSnake->direction = Snake::Direction::kRight;
  }
  else if (next_y < y)
  {
    computerSnake->direction = Snake::Direction::kUp;
  }
  else if (next_y > y)
  {
    computerSnake->direction = Snake::Direction::kDown;
  }
}

std::vector<std::vector<int>> Game::GetGrid()
{
  std::vector<std::vector<int>> grid(grid_height, std::vector<int>(grid_width, 0));

  for (const auto &point : snake->body)
  {
    int x = point.x;
    int y = point.y;
    if (x >= 0 && x < grid_width && y >= 0 && y < grid_height)
    {
      grid[y][x] = 1;
    }
  }

  for (const auto &point : computerSnake->body)
  {
    int x = point.x;
    int y = point.y;
    if (x >= 0 && x < grid_width && y >= 0 && y < grid_height)
    {
      grid[y][x] = 1;
    }
  }

  for (const auto &obstacle : obstacles)
  {
    int x = obstacle.x;
    int y = obstacle.y;

    if (x >= 0 && x < grid_width && y >= 0 && y < grid_height)
    {
      grid[y][x] = 1;
    }
  }

  for (const auto &obstacles : fixedObstacles)
  {
    for (const auto &position : obstacles.positions)
    {
      int x = position.first;
      int y = position.second;

      if (x >= 0 && x < grid_width && y >= 0 && y < grid_height)
      {
        grid[y][x] = 1;
      }
    }
  }

  return grid;
}