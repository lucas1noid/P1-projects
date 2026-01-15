#pragma once



#include <cstdint>
#include <string>
#include <vector>

#include "board.hpp"
#include "arguments.hpp"  

/// Class que representa o jogo em si e sua "vida".
class SudokuGame {
private:
  
  enum class game_state_e : std::uint8_t {
    WELCOME = 0, //! Boas vindas
    STARTING,    //! Tela inicial do jogo
    ENDING,      //! Finaliza o jogo
    HELPING,     //! Tela de ajuda
    QUITTING,    //! Verificação de saída
    PLAYING,     //! Jogando
    SAVING,      //! Salvando
    VICTORY,     //! Vitória
    GAMEOVER     //! Derrota
  };


  int m_errors;                        //! Quantidade de erros.
  game_state_e m_game_state;           //! Estado atual do jogo.
  std::vector<SudokuBoard> m_puzzles;  //! Lista de puzzles oferencidas.
  SudokuData m_sudoku_data;            //! Variável de manipulação de dados gerais do jogo.
  SudokuBoard m_working_board;         //!< cópia para sabermos qual tabuleiro esta sendo jogado
  bool inGaming;                       //! Aponta se já jogou alguma vez.
  std::string helper;                  //! Mensagem de help.
   int m_checks_left;                  //! Quantos checks faltam
  bool m_check_mode;                   //! Se deve pintar de verde/vermelho
  bool error_mode;                     //! Se deve pintar de vermelho.
  std::pair<int, int> m_last_move;     //! Onde desenhar as setas
  std::vector< std::pair<std::pair<int, int>, int> > undo_history;//! Histórico de jogadas para func undo

public:
  SudokuGame();
  ~SudokuGame() = default;
 
  /*!
   * @brief Lê os comandos durante o jogo.
   * @param m_working_board Puzzle que está sendo jogado.
   * @param arg Argumento a ser processado.
   */
  void readCommand(SudokuBoard& m_working_board, std::string arg);

  /*!
   * @brief Mostra uma mensagem.
   * @param error_msg Mensagem a ser exibida.
   */
  void usage(std::string error_msg) const;

  //! Funções gerais da máquina de estados.
  void update(); 
  void process_events();
  void render() const;
  void setup(const RunningOptions&);
  bool game_over();
  
  /*!
   * @brief Função para salvar o jogo serializando (binário)
   */
  bool save_game();

  /*!
   * @brief Função para carregar um jogo salvo desserializando (binário).
   */
  bool load_game();

};  

