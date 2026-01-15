cmake_minimum_required(VERSION 3.5)
project (Sudoku VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set (APP_NAME "sudoku")

add_executable(${APP_NAME} "main.cpp" "sudoku_gm.cpp" "board.cpp" "arguments.cpp")

target_compile_features( ${APP_NAME} PUBLIC cxx_std_17)
target_compile_options( ${APP_NAME} PRIVATE -Wall -pedantic -g)