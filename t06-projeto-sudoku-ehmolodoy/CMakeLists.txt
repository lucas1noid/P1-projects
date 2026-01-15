cmake_minimum_required(VERSION 3.40)
project (SudokuGame VERSION 1.0.0 LANGUAGES CXX )

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== ADDING SUBDIRECTORIES ===#
set ( APP_NAME "sudoku")
add_subdirectory(src)
