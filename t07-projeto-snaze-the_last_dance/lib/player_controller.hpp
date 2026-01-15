#pragma once
#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP

#include "controller.hpp"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

namespace sg
{
    class PlayerSnakeController : public SnakeController
    {
        public:
        PlayerSnakeController();
        ~PlayerSnakeController();
        
        sg::MoveDirection provide_snake_dir(const ControllerInput& state) override;

        private:
        char read_key();

        termios m_originalTerminal; 
    };
}

#endif // PLAYER_CONTROLLER_HPP
