#pragma once
#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <vector> // vector to hold th snake body
#include "common.hpp" // Position and move direction
#include "level.hpp"

/// @brief sg (Snake Game) is the default project namespace.
namespace sg 
{

    class Snake
    {
        public:
        Snake(const sg::Position startPos = sg::Position(0, 0), unsigned short lives = 3, unsigned short totalFood = 10);

        inline const sg::Position& head_position() const { return m_headPosition; }
        
        [[nodiscard]]
        inline const std::vector<sg::Position>& snake_body() const { return m_snakeBody; }

        void eat_food();

        void head_position(unsigned short y, unsigned short x);
        void head_position(const sg::Position& newPosition);

        [[nodiscard]]
        inline const sg::MoveDirection& move_direction() const { return m_headMoveDirection; }

        void move_direction(short y = 0, short x = 0);
        void move_direction(const sg::MoveDirection& newMoveDirection);

        void update_position_by_move_direction();

        [[nodiscard]] char get_head_char() const;

        [[nodiscard]] bool validate_position(const sg::Position& pos) const;

        bool head_collided(const sg::Maze& maze) const;
        
        void take_damage();
        bool is_dead() const;

        void reset_snake_food();
        void replace_snake();

        bool food_reached() const;

        void maze_update(const sg::Maze& maze);

        inline unsigned short get_lives() const { return m_lives; }

        [[nodiscard]]
        inline unsigned short get_food_eaten() const { return m_fruitsEaten; }

        inline unsigned short get_score() const { return m_score; }

        [[nodiscard]]
        inline unsigned short get_food_target() const { return m_totalFoodToEat; }

        private:
        bool m_creatingBodyFlag = false;

        sg::Position m_startHeadPosition;
        sg::Position m_headPosition;
        
        sg::MoveDirection m_headMoveDirection;
        std::vector<sg::Position> m_snakeBody;

        unsigned short m_lives;
        unsigned short m_totalFoodToEat;
        unsigned short m_fruitsEaten{0};
        unsigned short m_score{0};

    }; // Class Snake

} // Namespace sg

#endif // SNAKE_HPP
