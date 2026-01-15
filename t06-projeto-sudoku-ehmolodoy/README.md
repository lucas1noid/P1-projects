[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Twt2xWeD)
# Intro

This project implements a terminal version of the Sudoku game.

The **full description** of the project may be found [**here**](docs/sudoku_interativo.pdf).

# The game

According to the [wikipedia](https://en.wikipedia.org/wiki/Sudoku):

> Sudoku is a logic-based, combinatorial number-placement puzzle.
> The objective is to fill a 9×9 grid with digits so that each column, each row, and each of the nine 3×3 subgrids that compose the grid (also called "boxes", "blocks", or "regions") contain all of the digits from 1 to 9.
> The puzzle setter provides a partially completed grid, which for a well-posed puzzle has a single solution.

The Sudoku rules are:

1. Each row, column, and nonet can contain each number (typically 1 to 9) exactly once.
2. The sum of all numbers in any nonet, row, or column must be equal to 45.

# Input

A ascii file with puzzles, as the one available in [`data/puzzles.txt`](./data/puzzles.txt).

# Usage

```
% ./sudoku --help
Usage: sudoku [-c <num>] [-h] <input_cfg_file>
  Game options:
    -c  <num> Number of checks per game. Default = 3.
    -h        Print this help text.
```

# Features

The game offers:

- colored output.
- indication of _invalid_ plays, i.e. plays that breaks the Sudoku's basic rules.
- a simple interface.
- undo actions.
- limited possibility of checking if a move is correct or not.

# Author

UFRN/DIMAp, copyright 2020-2025.

Selan Santos
