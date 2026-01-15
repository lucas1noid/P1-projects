#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <algorithm>
#include <iostream>

/// @brief sg (Snake Game) is the default project namespace
namespace sg 
{
    /// @brief Struct that defines an object move direction
    struct MoveDirection
    {
        /// @brief A constructor for Move Direction struct.
        /// @param y the Y axis move direction.
        /// @param x the X axis move direction.
        explicit MoveDirection(short y = 0, short x = 0)
        : m_y(y), m_x(x)
        {}
        
        short m_y; //!< Move direction on Y Axis
        short m_x; //!< Move direction on X Axis

        inline bool operator==(const MoveDirection& other) const
        {
            return m_y == other.m_y && m_x == other.m_x;
        }

        inline bool equals(short y, short x) const
        {
            return m_y == y && m_x == x;
        }

        inline MoveDirection operator+(const MoveDirection& rhs) const
        {
            MoveDirection result(m_y + rhs.m_y, m_x + rhs.m_x);
            return result;
        }

        // If x and y move direction both are 0, the current direction is null i. e. the object is in idle state.
    }; // struct MoveDirection

    /// @brief Struct that defines an object position
    struct Position
    {
        /// @brief A constructor for Position struct.
        /// @param y the Y position, default value = 0.
        /// @param x the X position, default value = 0.
        explicit Position(unsigned short y = 0, unsigned short x = 0) :
        m_y(y), m_x(x){}

        unsigned short m_y; //!< Y Axis object position
        unsigned short m_x; //!< X Axis object position

        inline bool operator==(const Position& other) const
        {
            return m_y == other.m_y && m_x == other.m_x;
        }

        inline bool equals(unsigned short y, unsigned short x) const
        {
            return m_y == y && m_x == x;
        }

        inline Position operator+(const MoveDirection& rhs) const
        {
            Position result(m_y + rhs.m_y, m_x + rhs.m_x);
            return result;
        }
        // If x and y position both are 0, it means that the object is on the up-right corner. I. e. Y axis are fliped.
    }; // struct Position

    inline std::ostream& operator<<(std::ostream& os, const sg::Position pos)
    {
        os << "x: " << pos.m_x << " y: " << pos.m_y;
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const sg::MoveDirection mDir)
    {
        os << "x: " << mDir.m_x << " y: " << mDir.m_y;
        return os; 
    }

    /// @brief Trims leading and trailing characters from a string.
    /// @param input_str The string to trim.
    /// @param toRemove Characters to remove .
    /// @return Trimmed string.
    inline std::string trim_string(const std::string& input_str, const char* toRemove = " \t\n\r\f\v")
    {
        std::string result = input_str;
        result.erase(result.find_last_not_of(toRemove) + 1);
        result.erase(0, result.find_first_not_of(toRemove));
        return result;
    }

    /// @brief Converts a string to lowercase.
    /// @param str The string to convert.
    /// @return Lowercase version of the string.
    inline std::string str_to_lower(const std::string& str)
    {
        std::string result(str);
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    /// @brief Converts a string to uppercase.
    /// @param str The string to convert. 
    /// @return Uppercase version of the string.
    inline std::string str_to_upper(const std::string& str)
    {
        std::string result(str);
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

} // Namespace sg

#endif // COMMON_HPP
