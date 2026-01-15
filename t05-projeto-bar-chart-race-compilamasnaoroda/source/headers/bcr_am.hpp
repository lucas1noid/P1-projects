#pragma once
#include <string>
#include <cstdint>
#include <chrono>
#include "bar_chart.hpp"
#include "validate.hpp"

    //struct auxiliar para guardar um "ponto"
    struct Tick {
        long value; // O valor
        int pos;    // A posição na tela
    };

class AnimationController {
public:
    enum class state_e : std::uint8_t {
        start = 0,    //!< Estado inicial (antes da inicialização)
        welcome,     //!< Mostra mensagem de início
        reading,    //!< Lendo o arquivo csv
        racing,    //!< Rodando a animação no terminal
        end       //!< Final do programa
    };
    //construtor
    AnimationController();

    void initialize(int argc, char **argv);//!PASSAGEM DE ARGUMENTOS!

//maquina de estado
    bool is_over() const;
    void process_events();
    void update();
    void render();
    std::string get_color_categ(const std::string& category);

private:

    void print_usage();

    //var para guardar as configs
    int m_max_bars;
    int m_fps;
    std::string m_input_filename;

    Database m_database;

    state_e m_current_state;//estado

    IniMap m_configIni;//mapa de configuração

    int frame_index{};
    double time_per_frame{};
    double time_acumulator{};


    int last_rendered_frame{-1};//começa no -1 pq o primeiro frame é 0

    //serve pra medir o tempo desde o último update
    std::chrono::time_point<std::chrono::high_resolution_clock> time_s_lu;

    std::map<std::string, std::string> categ_to_color;
    std::vector<std::string> color_pallete;
    int next_color_index;

};
