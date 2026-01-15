/**!
 *  This program implements the Snake simulation.
 *  @author Selan R. dos Santos feat. noid e migs
 */


#include <iostream>
#include <chrono>
#include <thread>

//#include "simulation.hpp"
//#include "arguments.hpp"
#include "../lib/common.hpp"
#include "../lib/level.hpp"
#include "../lib/tcolor.hpp"
#include "../lib/snake.hpp"
#include "../lib/game.hpp"
#include "../lib/arguments.hpp"

int main(int argc, char* argv[]) {

  sg::GameOptions options;

  sg::ArgumentsReader argReader;
  argReader.parse(argc, argv, options);

    /*
  if (argc != 2)
  {
    std::cout << "invalid arguments\n";
    return 1;
  }
  */

  std::vector<sg::Maze> mazes = sg::parse_maze_input_file(options.levelPath);

  if (mazes.empty())
  {
    std::cout << ">> Error: No valid maze loaded from file: " << options.levelPath << "\n";
    std::cout << ">> Exiting...\n";
    return 1;
  }
  sg::Game game(mazes, options);
  game.start();

  return 0;
}