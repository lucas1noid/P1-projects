#pragma once
#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "common.hpp"
#include "snake.hpp"
#include "level.hpp"

namespace sg 
{
    struct ControllerInput
    {
        inline ControllerInput(const sg::Maze& maze, const sg::Snake& snake)
        : m_maze(maze), m_snake(snake) {}
        
        const sg::Maze& m_maze;
        const sg::Snake& m_snake;
    };

    class SnakeController
    {
        public:
        virtual ~SnakeController() = default;
        virtual sg::MoveDirection provide_snake_dir(const ControllerInput& state) = 0;
    };
}

#endif // CONTROLLER_HPP
