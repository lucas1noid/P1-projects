#include <iostream>
#include <string>
#include <vector>
#include <algorithm>//std::min std::max_element
#include <cmath>//std::round 
#include <iomanip>
#include <thread>
#include <chrono>//simular pausas(seconds, milliseconds)
#include "bcr_am.hpp"
#include "bar_chart.hpp"
#include "validate.hpp"

//strings de cores constantes
const std::string ANSI_RESET = "\033[0m";
const std::string ANSI_TITLE_COLOR = "\033[38;5;51m"; //ciano
const std::string ANSI_AXIS_COLOR = "\033[38;5;245m"; //cinza

/*!
 * Default constructor of the AnimationController class.
 *
 * @note Initializes default values ​​(max_bars, fps), color palette and initial state.
 */
AnimationController::AnimationController()
{
    m_max_bars = 5;
    m_fps = 24;
    last_rendered_frame = -1;
    next_color_index = 0; //inicializa o índice de cor

    color_pallete = {
        "\033[38;5;154m", // Verde Claro
        "\033[38;5;93m",  // Roxo
        "\033[38;5;214m", // Dourado
        "\033[38;5;196m", // Vermelho
        "\033[38;5;45m",  // Azul
        "\033[38;5;201m", // Magenta
        "\033[38;5;228m", // Amarelo
        "\033[38;5;252m"  // Branco/Cinza Claro
    };
    //caso chamada vazia, usa o construtor
}

///Prints program usage instructions (command-line options) to `std::cerr`.
void AnimationController::print_usage()
{
    std::cerr << "Usage: bcr [<options>] <input_data_file>\n"
              << "  Bar Chart Race options:\n"
              << "      -b  <num> Max # of bars in a single char.\n"
              << "                Valid range is [1,15]. Default values is 5.\n"
              << "      -f  <num> Animation speed in fps (frames per second).\n"
              << "                Valid range is [1,24]. Default value is 24." << std::endl;
}

/*!
 * Processes command line arguments, reads the .ini and loads the data file.
 *
 * @param argc The argument count.
 * @param argv The array of argument strings.
 * 
 * @note The program will exit if arguments or essential files are invalid, end call the usage.
 */
void AnimationController::initialize(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i]; // de argumento para string

        if (arg == "-b")
        {
            // o próximo argumento TEM QUE SER um número
            if (i + 1 < argc)
            {
                try
                {
                    m_max_bars = std::stoi(argv[++i]); // muda para int enquanto avança o índice ++i
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << ">>> Error: Invalid argument for option -b.\n";
                    print_usage();
                    exit(EXIT_FAILURE); // encerra o programa
                }
            }
        }
        else if (arg == "-f")
        {
            if (i + 1 < argc)
            {
                try
                {
                    m_fps = std::stoi(argv[++i]);
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << ">>> Error: Invalid argument for option -f.\n";
                    print_usage();
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            // caso não seja uma das opçoes, deve ser o nome do arquivo
            m_input_filename = arg;
        }
    }

    // após ler todos os argumentos, verificamos
    
    // nome
    if (m_input_filename.empty())
    {
        std::cerr << ">>> Error: Input data file is required.\n";
        print_usage();
        exit(EXIT_FAILURE);
    }

    //-b
    if (m_max_bars < 1 || m_max_bars > 15)
    {
        std::cerr << ">>> Error: Value for -b must be in the range [1,15].\n";
        print_usage();
        exit(EXIT_FAILURE);
    }

    //-f
    if (m_fps < 1 || m_fps > 24)
    {
        std::cerr << ">>> Error: Value for -f must be in the range [1,24].\n";
        print_usage();
        exit(EXIT_FAILURE);
    }


    //adicionei a parte que se conecta com o parameters.ini
    std::string iniFilename = "parameters.ini"; 
    //agr indo pra classe m_configIni
    m_configIni = parseIni(iniFilename);
    if (m_configIni.empty()) {
        std::cerr << ">>> Error: unable to load file: " << iniFilename << std::endl;
        exit(EXIT_FAILURE);
    }

    if (m_configIni.find("file_format") == m_configIni.end()) {
        std::cerr << ">>> Error: Section [file_format] not found in " << iniFilename << std::endl;
        exit(EXIT_FAILURE);
    }
    if (m_configIni.find("columns") == m_configIni.end()) {
        std::cerr << ">>> Error: Section [columns] not found in " << iniFilename << std::endl;
        exit(EXIT_FAILURE);
    }
    if (m_configIni.find("data_processing") == m_configIni.end()) {
        std::cerr << ">>> Error: Section [data_processing] not found in " << iniFilename << std::endl;
        exit(EXIT_FAILURE);
    }

    //chama a readfile e já preenche o database
    //agr passa m_configIni para a função readFile
    if (!readFile(m_input_filename, m_database, m_configIni)) {
        std::cerr << ">>> Error: unable to read and process file: " << m_input_filename << std::endl;
        exit(EXIT_FAILURE);
    }


    //para cada frame, vamos ordenar os dados das barras
    for (int i = 0; i < m_database.get_total_frames(); i++)
    {
        auto chart = m_database.get_frame(i);
        if (chart)
        {
            chart->sort_bars(); //se a função retornar um ponteiro não nulo, chama a função sort
        }
        
    }
    
    time_per_frame = 1000.0 / m_fps;

    frame_index = 0; //começa no primeiro frame
    time_acumulator = 0; 
    time_s_lu = std::chrono::high_resolution_clock::now(); //inicia o relógio

    // isso precisa ir na maquina de estados
    std::cout << ">>> Configuration loaded:\n"
              << "    + Max bars: " << m_max_bars << "\n"
              << "    + FPS: " << m_fps << "\n"
              << "    + Input file: " << m_input_filename << std::endl;

    m_current_state = state_e::welcome; // tudo certo, so falta dar enter
    std::cout << ">>> Press ENTER to start the animation..." << std::endl;
}

///Processes input events based on the m_current_state of the machine.
void AnimationController::process_events()
{
    // o que 'process_events' faz depende do estado em que o programa se encontra
    switch (m_current_state)
    {
    case (state_e::welcome):
        std::cin.get(); // esperando um ENTER do usuario
        m_current_state = state_e::racing;
        break;
    default:
        // start, reading, racing e end não precisam de input
        break;
    }
}

/*!
 * Checks if the animation should end.
 *
 * @return 'true' if the current state is 'end', 'false' otherwise.
 */
bool AnimationController::is_over() const
{
    return m_current_state == state_e::end;
} // pra quando termina a execucao

/*!
 * Updates animation logic, mainly timing and frame index.
 *
 * @note This function accumulates time (delta time) and advances to the next frame if the accumulated time exceeds `time_per_frame` (calculated from FPS).
 */
void AnimationController::update(){
    switch (m_current_state)
    {
    case (state_e::welcome):
        {//o programa travou na parte de dar enter
        break;
        }

    case (state_e::racing):
        {auto now = std::chrono::high_resolution_clock::now();
        auto delta_time = now - time_s_lu;
        time_s_lu = now; //reinicia para próxima vez

        double delta_ms = std::chrono::duration<double, std::milli>(delta_time).count();

        time_acumulator += delta_ms;

        if (time_acumulator >= time_per_frame)
        {
            time_acumulator -= time_per_frame;
            frame_index++;
        }

        if (frame_index >= m_database.get_total_frames())
        {
            m_current_state = state_e::end;
        }
        
        break;
    }
    case (state_e::end):
        {break;}

    case (state_e::start):
        {break;}
    case (state_e::reading): 
        {break;}

    default:
        break;
    }
}

/*!
* Gets an ANSI color for a given category string.
*
* @param category The category string ("Asia", "Europe").
*
* @return The string containing the associated ANSI color code.
*
* @note If the category is new or not allocated, assigns the next color in the palette and advances the index.
*/
std::string AnimationController::get_color_categ(const std::string& category){
    //se a categoria ainda não tem cor, atribui uma cor da paleta
    if (categ_to_color.find(category) == categ_to_color.end())
    {
        categ_to_color[category] = color_pallete[next_color_index];

        next_color_index = (next_color_index + 1) % color_pallete.size();
    }
    
    return categ_to_color[category];

}

/*!
* Renders the current frame of the animation to the terminal.
*
* @note This is the main display function. It only runs if the state is 'racing' and the current frame has not yet been rendered.
*/
void AnimationController::render() {

    if (m_current_state != state_e::racing) {
        return;
    }

    // Se já renderizamos este frame, não o imprimimos novamente.
    if (frame_index == last_rendered_frame) {
        return;
    }

    /*
    //parte comentada pois o prof pediu para não utilizar
    // códigos ANSI (funciona na maioria dos terminais Linux/macOS)
    std::cout << "\033[2J\033[1;1H";
    */

    //ponteiro pro pacote de dados do frame atual
    auto current_chart_ptr = m_database.get_frame(frame_index); //usa o frame_index atualizado pela update()
    
    //cabeçalho
    const auto& header_lines = m_database.get_header_lines();

    //pega a lista de elementos daquele pacote
    const auto& items = current_chart_ptr->get_items();

    //std::min para não tentar acessar mais itens do que existem no vetor
    size_t num_items_to_show = std::min((size_t)m_max_bars, items.size());

    //valor máximo entre os itens que serão mostrados
    long max_value = 0;
    if (num_items_to_show > 0) {
        max_value = items[0].value;
    }
    if (max_value <= 0) {
        max_value = 1; 
    }

    //calcular o tamanho do separador.
    auto value_to_original = [&](long value) -> double {
        double original_value = 0.0;
        try {
            double multiplier = std::stod(m_configIni.at("data_processing").at("value_multiplier"));
            if (multiplier != 0) {
                 if (multiplier == 1000) { 
                     original_value = (double)value;
                 } else {
                     original_value = (double)value / multiplier;
                 }
            } else {
                original_value = (double)value;
            }
        } catch (const std::exception &e) {
             original_value = (double)value;
        }
        return original_value;
    };

    const int MAX_BAR_WIDTH = 100; //largura da maior barra em caracteres (no meu 24pol. ficou bom)
    const std::string BAR_CHAR = "█";

    //imprime a 1 linha do cabeçalho como titulo, ou a padrão se não houver
    if (!header_lines.empty()) {
        std::cout << ANSI_TITLE_COLOR << header_lines[0] << ANSI_RESET << std::endl;
    } else {
        std::cout << ">>> Bar Chart <<<" << std::endl; // Fallback
    }
    
    std::cout << "Year: " << current_chart_ptr->get_moment() << std::endl;

    // Calcula o comprimento total da linha mais longa para o separador
    std::string max_value_str = std::to_string(static_cast<long>(value_to_original(max_value)));
    
    // 25 (label) + 3 ("|") + 100 (max bar) + 3 ("|") + comprimento do valor
    int total_line_width = 25 + 3 + MAX_BAR_WIDTH + 3 + max_value_str.length();
    
    if (total_line_width < 10) total_line_width = 10; 

    std::string separator_line(total_line_width, '-');
    std::cout << separator_line << std::endl;

    for (size_t i = 0; i < num_items_to_show; ++i) {
        const auto& item = items[i];

        int bar_width = static_cast<int>(std::round( (double)item.value / max_value * MAX_BAR_WIDTH ));
        bar_width = std::max(1, bar_width); //garante que a barra tenha pelo menos 1 caractere se o valor não for maior q zero

        std::string bar_string = "";
        for(int j=0; j < bar_width; ++j) {
            bar_string += BAR_CHAR;
        }

        //recupera o valor original antes da multiplicação
        double original_value = 0.0;
        try {
            double multiplier = std::stod(m_configIni.at("data_processing").at("value_multiplier"));
            if (multiplier != 0) {
                 if (multiplier == 1000) { //adapta conforme a lógica do multiplicador
                     original_value = (double)item.value; //para cities.csv
                 } else {
                     original_value = (double)item.value / multiplier; //outros casos como IDH
                 }

            } else {
                original_value = (double)item.value; //evitar bugs
            }
        } catch (const std::exception &e) {
             original_value = (double)item.value; //caso erro ao ler o .ini
        }

        // Imprime a linha: Label | Barra | Valor
        std::string bar_color = get_color_categ(item.category);

        //aplica a cor na barra-
        std::cout << std::left << std::setw(25) << item.label.substr(0, 24) // Limita o tamanho do label
                  << " | "
                  << bar_color // aplica a cor
                  << std::left << bar_string
                  << ANSI_RESET // reseta a cor
                  << " | "
                  // formata o valor original. Para população, sem casas decimais.
                  << "[" << std::fixed << std::setprecision(0) << original_value << "]"
                  << '\n' << std::endl;
    }
    
    const int MIN_TICK_SEPARATION = 10;

    //pegamos o min e max DO FRAME ATUAL
    long min_value = items[num_items_to_show - 1].value;
    
    std::vector<Tick> ticks;
    
    //0
    ticks.push_back({0, 0}); 

    //pos min
    int min_pos = static_cast<int>(std::round((double)min_value * MAX_BAR_WIDTH / max_value));
    ticks.push_back({min_value, min_pos});

    //pos max
    ticks.push_back({max_value, MAX_BAR_WIDTH}); 

    //mid
    long mid_value = (min_value + max_value) / 2;
    int mid_pos = static_cast<int>(std::round((double)mid_value * MAX_BAR_WIDTH / max_value));
    ticks.push_back({mid_value, mid_pos});
    
    //ordena pela posição na tela e evita sobreposição
    std::sort(ticks.begin(), ticks.end(), [](const Tick& a, const Tick& b) {
        return a.pos < b.pos;
    });

    // remove ticks muito próximos
    std::vector<Tick> clean_ticks;
    if (!ticks.empty()) {
        clean_ticks.push_back(ticks[0]); //add tick 0
        for (size_t i = 1; i < ticks.size(); ++i) {
            if ((ticks[i].pos - clean_ticks.back().pos) >= MIN_TICK_SEPARATION) {
                clean_ticks.push_back(ticks[i]);
            }
            //garante que o max_value seja sempre adicionado
            else if (i == ticks.size() - 1 && clean_ticks.back().pos != ticks[i].pos) {
                clean_ticks.pop_back(); //remove o anterior
                clean_ticks.push_back(ticks[i]); //add max
            }
        }
    }

    //buffer""
    std::string label_line(MAX_BAR_WIDTH + 20, ' ');
    std::string tick_line_dotted(MAX_BAR_WIDTH, '.');//linha pontilhada

    std::cout << std::endl << ANSI_AXIS_COLOR; // ativa a cor do eixo

    for (const auto& tick : clean_ticks) {
        //converte o valor para string printável
        std::string label = std::to_string(static_cast<long>(value_to_original(tick.value)));
        
        if (tick.pos + label.length() < label_line.length()) {
            label_line.replace(tick.pos, label.length(), label);
        }
        
        //"ponto" na linha pontilhada
        if (static_cast<size_t>(tick.pos) < tick_line_dotted.length()) {
            tick_line_dotted[tick.pos] = '|';
        }
    }
    
    std::string axis_prefix(25, ' '); // 25 espaços para o label
    axis_prefix += " | ";             // 3 caracteres para o separador
    
    // Imprime as linhas do eixo
    std::cout << axis_prefix << label_line.substr(0, MAX_BAR_WIDTH + 10) << std::endl;
    std::cout << axis_prefix << tick_line_dotted.substr(0, MAX_BAR_WIDTH + 1) << std::endl;

    std::cout << ANSI_RESET << std::endl; //reseta a cor

    // Imprime a 3 linha do cabeçalho (Source), se ela existir
    if (header_lines.size() >= 3) {
        std::cout << header_lines[2] << std::endl;
    }

    std::cout << "Label: ";
    //pair.first é a categoria e pair.second é a cor
    for (const auto& pair : categ_to_color) {
        std::cout << pair.second << BAR_CHAR << ANSI_RESET 
                  << " : " << pair.first << "   ";
    }
    std::cout << std::endl;

    //já foi renderizado para evitar repetições
    last_rendered_frame = frame_index;
}