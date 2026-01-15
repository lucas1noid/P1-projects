#include "../lib/snake.hpp"

sg::Snake::Snake(const sg::Position startPos, unsigned short lives, unsigned short foodToEat)
: m_startHeadPosition(startPos), m_lives(lives), m_totalFoodToEat(foodToEat),
m_creatingBodyFlag(false), m_fruitsEaten(0), m_headMoveDirection(sg::MoveDirection(0, 0)), m_headPosition(startPos), m_snakeBody()
{}

void sg::Snake::head_position(unsigned short y, unsigned short x)
{
    m_headPosition.m_y = y;
    m_headPosition.m_x = x;
}

void sg::Snake::head_position(const sg::Position& newPosition)
{
    head_position(newPosition.m_y, newPosition.m_x);
}

void sg::Snake::move_direction(short y, short x)
{
    m_headMoveDirection.m_y = y;
    m_headMoveDirection.m_x = x;
}

void sg::Snake::move_direction(const sg::MoveDirection& newMoveDir)
{
    move_direction(newMoveDir.m_y, newMoveDir.m_x);
}

void sg::Snake::update_position_by_move_direction()
{
    if (m_headMoveDirection.equals(0, 0))
    {
        return;
    }

    if (m_creatingBodyFlag) 
    {
        m_snakeBody.insert(m_snakeBody.cbegin(), m_headPosition);
        m_creatingBodyFlag=false;
    }
    else
    {
        std::size_t bodySize = m_snakeBody.size();
        if (bodySize > 0)
        {
            for (std::size_t part{bodySize-1}; part > 0; --part)
            {
                m_snakeBody[part] = m_snakeBody[part-1]; 
            }
            m_snakeBody[0] = m_headPosition;
        }
    }
    

    m_headPosition.m_y += m_headMoveDirection.m_y;
    m_headPosition.m_x += m_headMoveDirection.m_x;
}

void sg::Snake::eat_food()
{
    m_creatingBodyFlag = true;
    ++m_fruitsEaten;
    ++m_score;
}

char sg::Snake::get_head_char() const
{
    short x = m_headMoveDirection.m_x;
    short y = m_headMoveDirection.m_y;

    if (y == 0)
    {
        return x >= 0 ? '>' : '<'; 
    }

    return y >= 0 ? 'v' : '^';
}

bool sg::Snake::validate_position(const sg::Position& pos) const
{
    if (m_headPosition == pos)
    {
        return false;
    }

    for (const sg::Position& bodyPart : m_snakeBody)
    {
        if (bodyPart == pos)
        {
            return false;
        }
    }

    return true;
}

bool sg::Snake::head_collided(const sg::Maze& maze) const
{
    for (const sg::Position& body : m_snakeBody)
    {
        if (m_headPosition == body)
        {
            return true;
        }
    }

    if (!maze.validate_position(m_headPosition))
    {
        return true;
    }

    return false;
}

void sg::Snake::take_damage()
{
    if (m_lives <= 0)
    {
        return;
    }

    --m_lives;
}

bool sg::Snake::is_dead() const
{
    return m_lives == 0;
}

void sg::Snake::reset_snake_food()
{
    m_creatingBodyFlag = false;
    m_snakeBody.clear();
}

bool sg::Snake::food_reached() const
{
    return m_fruitsEaten >= m_totalFoodToEat;
}

void sg::Snake::replace_snake()
{
    reset_snake_food();
    m_headPosition = m_startHeadPosition;
    m_headMoveDirection = sg::MoveDirection(0, 0);
}

void sg::Snake::maze_update(const sg::Maze& maze)
{
    reset_snake_food();
    m_startHeadPosition = maze.get_start_position();
    m_headPosition = maze.get_start_position();
    m_headMoveDirection = sg::MoveDirection(0, 0);
    m_fruitsEaten = 0;
}