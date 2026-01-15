cmake_minimum_required(VERSION 4.0)
project(PassGen VERSION 1.0.0 LANGUAGES CXX)

# Generation of compilation databases for Unix Makefile builds.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Setting variables
set(APP_NAME "passgen")

# Main App
add_executable(${APP_NAME} src/main.cpp src/validate.cpp src/generator.cpp src/entropy.cpp) #ADD ANOTHER CPP

# Building options
target_compile_features( ${APP_NAME} PUBLIC cxx_std_17 )
target_compile_options(${APP_NAME} PRIVATE -Wall -pedantic -g)
# target_compile_options( ${APP_NAME} PRIVATE -Wall -pedantic -fstandalone-debug -g)
