#include "../lib/player_controller.hpp"

sg::PlayerSnakeController::PlayerSnakeController() : sg::SnakeController()
{
    tcgetattr(STDIN_FILENO, &m_originalTerminal); // GET ORIGINAL TERMINAL
            
    // Configura o novo modo
    termios raw = m_originalTerminal;
    raw.c_lflag &= ~(ICANON | ECHO); // Desliga buffer de linha e echo na tela
    raw.c_cc[VMIN] = 0;  // Não espera nenhum caractere mínimo para retornar
    raw.c_cc[VTIME] = 0; // Não espera tempo
    
    // Aplica
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    // Configura leitura não-bloqueante (fcntl)
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

sg::PlayerSnakeController::~PlayerSnakeController()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &m_originalTerminal);
}

sg::MoveDirection sg::PlayerSnakeController::provide_snake_dir(const ControllerInput& state)
{
    char key = read_key();
    sg::MoveDirection result = state.m_snake.move_direction();

    if (key == 'w')
    {
        result = sg::MoveDirection(-1, 0);
    }

    if (key == 'a')
    {
        result = sg::MoveDirection(0, -1);
    }

    if (key == 's')
    {
        result = sg::MoveDirection(1, 0);
    }

    if (key == 'd')
    {
        result = sg::MoveDirection(0, 1);
    }

    if ((result + state.m_snake.move_direction()).equals(0, 0))
    {
        return state.m_snake.move_direction();
    }

    return result;
}

char sg::PlayerSnakeController::read_key()
{
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) > 0) {
        return c;
    }
    return 0; 
}