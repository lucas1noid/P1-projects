#pragma once
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string> // String import.
#include <vector> // Vector import.
#include "common.hpp" // Position import.

/// @brief sg (Snake Game) is the default project namespace.
namespace sg 
{
    /// @brief A class that represents a maze.
    class Maze
    {
        public:
        Maze() :
        m_charPosition(), m_startPosition() {}

        Maze(const std::vector<std::vector<char>>& charPosition, const Position& startPosition, unsigned short y, unsigned short x)
        : m_charPosition(charPosition), m_startPosition(startPosition), m_ySize(y), m_xSize(x){} 

        /// @brief It recieves a position and return if the position is valid or not.
        /// @param position The position to check.
        /// @return True if the position is valid, false otherwhise.
        [[nodiscard]] bool validate_position(const Position& position) const;

        /// @brief Getter for start position of the maze
        /// @return A reference for the start position of the maze
        inline const Position& get_start_position() const { return m_startPosition; }

        /// @brief Getter for height of the maze
        /// @return Height value of the maze
        inline unsigned short get_y_size() const { return m_ySize; }

        /// @brief Getter for width of the maze
        /// @return Width value of the maze
        inline unsigned short get_x_size() const { return m_xSize; }

        std::string position_char(const sg::Position& pos) const;

        private:
        std::vector<std::vector<char>> m_charPosition; //!< A 2x2 Matrix that holds if a position is valid or not.
        Position m_startPosition; //!< The maze start position.
        unsigned short m_ySize; //!< The maze Y axis size.
        unsigned short m_xSize; //!< The maze X axis size. 

    }; // class Maze

    std::vector<Maze> parse_maze_input_file(const std::string& mazeFilePath);
} // Namespace sg

#endif // LEVEL_HPP