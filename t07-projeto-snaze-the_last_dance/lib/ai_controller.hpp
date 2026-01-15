#pragma once
#ifndef AI_CONTROLLER_HPP
#define AI_CONTROLLER_HPP

#include "controller.hpp"
#include <random>
#include <stack>

namespace sg
{
    
    class RandomMoveController : public sg::SnakeController
    {
        public:
        sg::MoveDirection provide_snake_dir(const sg::ControllerInput&) override;
        
        private:
        std::random_device m_dev;
    };

    class BacktrackingMoveController : public sg::SnakeController
    {
        public:
        sg::MoveDirection provide_snake_dir(const sg::ControllerInput&) override;
        
        private:
        std::stack<sg::MoveDirection> m_movePath;
    };

};


#endif // AI_CONTROLLER_HPP
