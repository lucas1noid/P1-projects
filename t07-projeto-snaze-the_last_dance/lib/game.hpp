#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "snake.hpp"
#include "level.hpp"
#include "arguments.hpp"
#include "player_controller.hpp"

#include <chrono>
#include <memory>

namespace sg
{

    enum class PlayerType 
    {
        HUMAN,
        AI
    };
    
    class Game
    {
        public:
        Game(const std::vector<sg::Maze>& mazes, const GameOptions& options);

        [[nodiscard]] std::vector<sg::Position> get_valid_positions() const;
        [[nodiscard]] inline const std::vector<sg::Position>& get_fruits_positions() const { return m_fruits; }

        [[nodiscard]] bool validate_position(const sg::Position& position) const;

        [[nodiscard]] inline const sg::Maze& get_current_maze() const { return m_allMazes[m_currentMazeIndex]; }

        void start();
        void update();
        bool set_maze(std::size_t mazeIndex);
        bool next_maze();
        bool spawn_food();

        private:
        sg::Snake m_currentSnake;
        std::vector<sg::Position> m_fruits;

        std::size_t m_currentMazeIndex{0};
        std::vector<sg::Maze> m_allMazes;
        PlayerType m_playerType{PlayerType::HUMAN};
        
        std::unique_ptr<sg::SnakeController> m_controllerStrategy;
        //tirei elas do construtor aq no hpp e mandei pro cpp

        std::chrono::milliseconds m_frameDuration;

        void render() const;
        void welcome_screen();
        void lose_screen();
        void win_screen();
    }; // Class SnakeGame

} // Namespace sg

#endif // GAME_HPP