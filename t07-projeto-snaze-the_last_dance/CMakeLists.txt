cmake_minimum_required(VERSION 3.5)
project (snazeGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set (APP_NAME "snaze")

add_executable(${APP_NAME} 
"src/main.cpp" 
"src/arguments.cpp" 
"src/level.cpp"
"src/snake.cpp"
"src/game.cpp"
"src/player_controller.cpp"
"src/ai_controller.cpp"
)

target_compile_features( ${APP_NAME} PUBLIC cxx_std_17)
target_compile_options( ${APP_NAME} PRIVATE -Wall -pedantic -g)