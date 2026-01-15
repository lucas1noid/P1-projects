#include "../lib/game.hpp"
#include "../lib/tcolor.hpp"
#include "../lib/ai_controller.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <random>

sg::Game::Game(const std::vector<sg::Maze>& mazes, const GameOptions& options)
: m_allMazes(mazes),
  m_currentMazeIndex(0)
{
    //transforma fps em tempo de frame
    int fps = (options.fps > 0) ? options.fps : 1;
    m_frameDuration = std::chrono::milliseconds(1000 / fps);
    m_currentSnake = sg::Snake(sg::Position(), options.lives, options.food);
}

std::vector<sg::Position> sg::Game::get_valid_positions() const
{
    std::vector<sg::Position> result;

    if (m_currentMazeIndex >= m_allMazes.size())
    {
        return result;
    }

    for (unsigned short y{0}; y < get_current_maze().get_y_size(); ++y)
    {
        for (unsigned short x{0}; x < get_current_maze().get_x_size(); ++x)
        {
            Position currentPosition(y, x);

            if (!validate_position(currentPosition))
            {
                continue;
            }

            result.push_back(currentPosition);
        }
    }

    return result;
}

bool sg::Game::validate_position(const sg::Position& position) const
{
    if (!get_current_maze().validate_position(position))
    {
        return false;
    }

    if (!m_currentSnake.validate_position(position))
    {
        return false;
    }

    for (const sg::Position& f : m_fruits)
    {
        if (f == position)
        {
            return false;
        }
    }

    return true;
}

bool sg::Game::set_maze(std::size_t index)
{
    if (index >= m_allMazes.size())
    {
        return false;
    }
    
    m_currentMazeIndex = index;
    return true;
}

bool sg::Game::next_maze()
{
    return set_maze(m_currentMazeIndex + 1);
}

void sg::Game::lose_screen()
{
        //std::cout << "\033[2J\033[1;1H";

        std::cout << TColor::colorize("-----------------------------------------------------------------------\n", TColor::RED, TColor::BOLD);
        std::cout << TColor::colorize("|              Y O U  H A V E  L O S T  T H E  G A M E                |\n", TColor::RED, TColor::BOLD);
        std::cout << TColor::colorize("-----------------------------------------------------------------------\n\n", TColor::RED, TColor::BOLD);
        std::cout << TColor::colorize("                B E T T E R  L U C K  N E X T  T I M E !                     \n\n", TColor::BRIGHT_BLACK, TColor::BLINK);
}

void sg::Game::win_screen()
{
        //std::cout << "\033[2J\033[1;1H";

        std::cout << TColor::colorize("-----------------------------------------------------------------------\n", TColor::GREEN, TColor::BOLD);
        std::cout << TColor::colorize("|              Y O U  H A V E  W O N  T H E  G A M E                  |\n", TColor::GREEN, TColor::BOLD);
        std::cout << TColor::colorize("-----------------------------------------------------------------------\n\n", TColor::GREEN, TColor::BOLD);
        std::cout << TColor::colorize("                     C O N G R A T U L A T I O N S !                     \n\n", TColor::BRIGHT_BLACK, TColor::BLINK);
}

void sg::Game::welcome_screen()
{
    bool validChoice = false;
    char choice;

    while (!validChoice)
    {
        std::cout << "\033[2J\033[1;1H";

        std::cout << TColor::colorize("--------------------------------------------------\n", TColor::BLUE, TColor::BOLD);
        std::cout << TColor::colorize("|              S N A Z E   G A M E               |\n", TColor::BLUE, TColor::BOLD);
        std::cout << TColor::colorize("--------------------------------------------------\n\n", TColor::BLUE, TColor::BOLD);
        
        std::cout << "Levels carregados: " << TColor::colorize(std::to_string(m_allMazes.size()), TColor::GREEN, TColor::BLINK) << "\n\n";

        std::cout << "   Escolha o modo de jogo:\n\n";

        std::cout << "   [" << TColor::colorize("1", TColor::YELLOW, TColor::BOLD) << "] - " 
                  << TColor::colorize("Jogador", TColor::GREEN) << "\n";
        std::cout << "       Controle a cobra com W, A, S, D.\n\n";

        std::cout << "   [" << TColor::colorize("2", TColor::YELLOW, TColor::BOLD) << "] - " 
                  << TColor::colorize("IA", TColor::MAGENTA) << "\n";
        std::cout << "       Assista a IA jogar sozinha, tentando ganhar! ou lutando para sobreviver...\n\n";

        std::cout << "   [" << TColor::colorize('q', TColor::RED, TColor::BOLD) << "] - "
                  << TColor::colorize("Sair", TColor::RED) << "\n\n";

        std::cout << "   Digite sua opcao: ";

        std::cin >> choice;

        if (choice == '1')
        {
            m_playerType = PlayerType::HUMAN;
            m_controllerStrategy = std::make_unique<sg::PlayerSnakeController>();
            validChoice = true;
        }
        else if (choice == '2')
        {
            m_playerType = PlayerType::AI;
            m_controllerStrategy = std::make_unique<sg::RandomMoveController>();
            validChoice = true;
        }
        else if (choice == 'q')
        {
            std::cout << "Até a próxima! Saindo...\n";
            std::exit(0);
        }
        else
        {
            std::cout << TColor::colorize("\n   Opcao invalida! Tente novamente.\n", TColor::RED);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            
            std::cin.ignore(1000, '\n');
            std::cin.clear();
        }
    }

    //feedback antes de comecar
    std::cout << "\n   Iniciando jogo no modo: " 
              << (m_playerType == PlayerType::HUMAN ? "HUMANO" : "IA") 
              << "...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void sg::Game::start()
{
    if (m_allMazes.size() <= 0)
    {
        return;
    }

    welcome_screen();

    set_maze(0);
    m_currentSnake.maze_update(get_current_maze());
    
    bool startGame = true;
    while (!spawn_food())
    {
        bool hasMaze = next_maze();
        if (!hasMaze)
        {
            std::cout << "Em todos os mapas carregados, não há espaço para colocar comidas. Não é possível iniciar o jogo.\n";
            startGame = false;
            break;
        }

        m_currentSnake.maze_update(get_current_maze());
    }
    if (!startGame)
    {
        return;
    }
    update(); 
}

void sg::Game::update()
{
    while (1)
    {
        render();

        sg::ControllerInput context(get_current_maze(), m_currentSnake);

        sg::MoveDirection newMoveDirection = m_controllerStrategy.get()->provide_snake_dir(context);
        m_currentSnake.move_direction(newMoveDirection);
        m_currentSnake.update_position_by_move_direction();

        if (m_currentSnake.head_collided(get_current_maze()))
        {
            
            m_currentSnake.take_damage();
            render();
            if (m_currentSnake.is_dead())
            {
                lose_screen();
                break;
            }
            else
            {
                m_currentSnake.replace_snake();
            }
        }

        for (std::size_t fIndex{0}; fIndex < m_fruits.size(); ++fIndex)
        {
            if (m_fruits[fIndex] == m_currentSnake.head_position())
            {
                m_currentSnake.eat_food();
                m_fruits.erase(m_fruits.begin() + fIndex);
                if (!spawn_food())
                {
                    render();
                    win_screen();
                    break;
                }
            }
        }

        if (m_currentSnake.food_reached())
        {
            if (next_maze())
            {
                m_currentSnake.maze_update(get_current_maze());
                m_currentSnake.head_position(get_current_maze().get_start_position());
                m_fruits.clear();
                if (!spawn_food())
                {
                    render();
                    win_screen();
                    break;
                }
            }
            else
            {
                render();
                win_screen();
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(m_frameDuration));
    }

}

bool sg::Game::spawn_food()
{
    std::vector<sg::Position> validPosition = get_valid_positions();
    if (validPosition.size() <= 0)
    {
        return false;
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, validPosition.size());
    int index = dist(rng)%validPosition.size();
    m_fruits.push_back(validPosition[index]);
    return true; 
}

void sg::Game::render() const
{
    if (m_currentMazeIndex >= m_allMazes.size())
    {
        return;
    }

    std::cout << "\033[2J\033[1;1H";

    for (size_t y{0}; y < get_current_maze().get_y_size(); ++y)
    {
        for (size_t x{0}; x < get_current_maze().get_x_size(); ++x)
        {
            Position currentPos = Position(y, x);
            bool charPrinted = false;
            for (const sg::Position& fruit : m_fruits)
            {
                if (fruit == currentPos)
                {
                    std::cout << TColor::colorize('Q', TColor::RED, TColor::BOLD);
                    charPrinted = true;
                    break;
                }
            }

            if (charPrinted)
            {
                continue;
            }

            for (const sg::Position& snakeBody : m_currentSnake.snake_body())
            {
                if (snakeBody == currentPos)
                {
                    std::cout << TColor::colorize(' ', TColor::GREEN, TColor::REVERSE);
                    charPrinted = true;
                    break;
                }
            }

            if (charPrinted)
            {
                continue;
            }

            if (m_currentSnake.head_position() == currentPos)
            {
                std::cout << TColor::colorize(m_currentSnake.get_head_char(), TColor::GREEN, TColor::BOLD);
                continue;
            }

            std::cout << get_current_maze().position_char(currentPos);
        }

        std::cout << '\n';
    }

    std::cout << "--------------------------------------------------\n";
    std::cout << "Score: " << m_currentSnake.get_score() * 10 << " | "
              << "Lives: " << m_currentSnake.get_lives()
              << " | Fruits Eaten: " << m_currentSnake.get_food_eaten() << "/" << m_currentSnake.get_food_target()
              << " | Snake Size: " << m_currentSnake.snake_body().size() << '\n';
    std::cout << "Move direction: [" << m_currentSnake.move_direction() << "]\n";
    std::cout << "Head position: [" << m_currentSnake.head_position() << "]\n";
}