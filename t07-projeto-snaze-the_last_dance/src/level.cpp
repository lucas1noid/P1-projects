#include "../lib/level.hpp"
#include "../lib/tcolor.hpp"

#include <filesystem> // Import for validate path
#include <fstream> // Import for read files
#include <sstream> // Import for read strings

std::vector<sg::Maze> sg::parse_maze_input_file(const std::string& mazeFilePath)
{
    std::vector<sg::Maze> result; // Result holder

    if (!std::filesystem::is_regular_file(mazeFilePath )) // Checks if the input path is valid
    {
        return result; // Input path invalid, returning empty 
    }

    std::ifstream file(mazeFilePath); // Create a Input File Stream with the path
    if (!file.is_open()) // If the file could not open
    {
        return result; // Something went wrong at the opening file process
    }

    std::string currentLine; // Buffer for read lines
    while (std::getline(file, currentLine)) // While there are lines in the file
    {
        currentLine = sg::trim_string(currentLine); // Current line trimmed
        if (currentLine.empty()) // If the current line is empty are trimmed
        {
            continue; // Back to the top of the while loop
        }

        std::string numbers; // String to hold the numbers
        std::istringstream iss(currentLine); // Input string stream to read numbers

        int rowsInFile; // Rows for the next maze
        int colsInFile; // Cols for the next maze
        if (!(iss >> rowsInFile >> colsInFile)) // If invalid format i. e. fail trying to set rows and cols 
        {
            continue; // Back to the top of the while loop
        }

        if (rowsInFile <= 0 || rowsInFile > 100) // If the rows is out of the expected range
        {
            continue; // Back to the top of the while loop
        }

        if (colsInFile <= 0 || colsInFile > 100) // If the cols is out of the expected range
        {
            continue; // Back to the top of the while loop
        }
        
        std::vector<std::vector<char>> validPosition(rowsInFile); // Creates the valid position matrix
        sg::Position startPosition; // Start position buffer
        bool startPosSetFlag = false; // A flag to check if the start position was set
        bool errorInParseProcess = false; // A flag to check if the read process failed

        for (int row{0}; row < rowsInFile; ++row) // For each line read
        {
            if (!std::getline(file, currentLine) || currentLine.size() < colsInFile) // If the get line failed or the current line has less chars than expected
            {
                errorInParseProcess = true; // error in parse flag active
                break; // Breakes the read process for the current maze
            }  
            
            for (int col{0}; col < colsInFile; ++col) // For each char in line read
            {
                char lineChar = currentLine[col]; // get the chat at xIt position
                if (lineChar == '&') // Check if the caracter is & i. e. the char that indicates the start position
                {
                    if (startPosSetFlag) // If the start position was set before
                    {
                        errorInParseProcess = true; // error in parse flag active
                        break; // Break the read process for the current maze
                    }
                    startPosition = sg::Position(row, col); // Set the start position
                    startPosSetFlag = true; // Set the start pos flag as true
                    validPosition[row].push_back('&');
                    continue; // Back to the top of the X for loop i. e. the next char
                }

                if (lineChar != ' ' && lineChar != '#' && lineChar != '.') // White character indicates that the position is free
                {
                    errorInParseProcess = true; // error in parse flag active
                    break; // Break the read process for the current maze
                }

                validPosition[row].push_back(lineChar); // Set the current position as valid if the char is white space, false otherwise.
            }

            if (errorInParseProcess) // If the error in parse flag was active at the line parse process
            {
                break; // Breakes the read process for the current maze
            }
        }

        if (errorInParseProcess || !startPosSetFlag) // If read complete flag is false or not start pos was set
        {
            continue; // Back to the top of the while loop
        }

        sg::Maze newMaze(validPosition, startPosition, rowsInFile, colsInFile); // Create a new maze with the information read
        result.push_back(newMaze); // Add the maze at the result vector
    }

    return result; // Return the result after the parse process.
}

bool sg::Maze::validate_position(const sg::Position& position) const
{
    if (position.m_y >= m_charPosition.size()) // If the y position is out of range 
    {
        return false; // The position is invalid, returning false
    }

    if (position.m_x >= m_charPosition[position.m_y].size()) // If the x position is out of range
    {
        return false; // The position is invalid, returning false
    }

    char posChar = m_charPosition[position.m_y][position.m_x];

    return posChar == ' ' || posChar == '&'; // Return the value at y, x position from matrix
}

std::string sg::Maze::position_char(const sg::Position& pos) const
{
    char posChar = m_charPosition[pos.m_y][pos.m_x];
    
    if (posChar == '#')
    {
        return TColor::colorize(' ', TColor::WHITE, TColor::REVERSE);
    }
    return TColor::colorize(' ', TColor::WHITE, TColor::REGULAR);
}