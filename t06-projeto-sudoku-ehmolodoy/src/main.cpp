/**
 * @file main.cpp
 *
 * @description
 * This program implements an interactive Sudoku game.
 *
 * ===========================================================================
 * @license
 *
 * This file is part of SUDOKU GAME project.
 *
 * SUDOKU GAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SUDOKU GAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SUDOKU GAME.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2014-2025 Selan R. dos Santos
 * ===========================================================================
 *
 * @author	Selan Santos, <selan.santos@ufrn.br>
 * @date	2025
 *
 * @remark On 2025-09-19 changed ...
 */

#include <cstdlib>  // EXIT_SUCCESS

#include "arguments.hpp"  // contains parse_cli_arguments(...).
#include "sudoku_gm.hpp"   // contains SudokuGame class definition.

int main(int argc, char** argv) {
  RunningOptions runop = parse_cli_arguments(argc, argv);
  SudokuGame game;
  // Set up simulation.
  game.setup(runop);
  // The Game Loop (Architecture)
  while (not game.game_over()) {
    game.process_events();
    game.update();
    game.render();
  }
  return EXIT_SUCCESS;
}
