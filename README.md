# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

## New Features

1. After the game ends, you can save your score. The top 10 highest scores will be displayed.

  - Loops, Functions, I/O - meet at least 2 criteria
    - A variety of control structures are added to the project.
    - The project code is clearly organized into functions.
    - The project reads data from an external file or writes data to a file as part of the necessary operation of the program.
    
2. Fixed and moving obstacles have been added to the game.
  - Every time you eat food, a fixed obstacle will be generated in a random location.
  - Moving obstacles will be generated in random locations at every 5-point intervals, such as 
  at 5, 10, and 15 points.

  - Object Oriented Programming - meet at least 3 criteria
    - Classes are organized with attributes to hold data and methods to perform tasks.
    - All class members that are set to argument values are initialized through member initialization lists.
    - All class member functions document their effects, either through function names, comments, or formal documentation.

3. Various types of food have been added to the game.
  - Special food has been added. If there is no special food, it will be generated every 10 seconds. When you eat special food, all obstacles will change into regular food.
  - Special food moves randomly up, down, left, or right every second.

  - Concurrency - meet at least 2 criteria
    - The project uses multiple threads or async tasks in the execution.
    - A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.

4. Another snake controlled by the computer has been added to the game using the A* search algorithm. 
  - Memory Management - meet at least 3 criteria
    - At least two variables are defined as references, or two functions use pass-by-reference in the project code.
    - The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr.
    - The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
