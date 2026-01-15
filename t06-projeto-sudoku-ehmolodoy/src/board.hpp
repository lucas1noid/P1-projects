#pragma once

#include "arguments.hpp"
#include <memory>
#include <vector>
#include <utility>
#include <fstream>

using value_type = short;


/// Classe que guarda valores do tabuleiro de Sudoku 9x9.
class SudokuBoard {
public:

  static constexpr value_type max_dim{ 9 };

  value_type m_board[max_dim][max_dim];
  bool m_fixed[max_dim][max_dim];//verificar se a celula é fixa 
  
  ///@brief Construtor padrão que inicializa o tabuleiro.
  SudokuBoard();

  /*!
   * @brief Coloca a jogada informada pelo jogador.
   * @param row Linha da jogada.
   * @param col Coluna da jogada.
   * @param value Valor a ser colocado na coodenada selecionada.
   */
  void set_cell(int row, int col, value_type value);
  /*!
   * @brief Remove o valor na célula informada pelo jogador.
   * @param row Linha da jogada.
   * @param col Coluna da jogada.
   */
  void remove(int row, int col); 
  /*!
   * @brief Exibe o tabuleiro.
   * @param original Tabuleiro original que foi lido.
   * @param check_mode Verifica se está no modo check (verificação).
   * @param last_move Coordenadas do ultima jogada.
   * @param error Verifica se está em erro.
   */
  void print(const SudokuBoard* original = nullptr, bool check_mode = false, std::pair<int, int> last_move = {-1,-1}, bool error = false) const;
  /*!
   * @brief Verifica se o valor da célula é um número fixo (não pode ser alterado).
   * @param row Linha da jogada.
   * @param col Coluna da jogada.
   */
  bool is_fixed(int row, int col) const; 
  /*!
   * @brief Verifica se a jogada é válida.
   * @param row Linha da jogada.
   * @param col Coluna da jogada.
   * @param value Valor a ser colocado na coodenada selecionada.
   */
  [[nodiscard]] bool is_safe(int row, int col, value_type value) const; 
  /*!
   * @brief Verifica se o jogo foi completamente resolvido.
   * @param original Tabuleiro original.
   */
  bool solved(const SudokuBoard& original) const;
  
  /*!
   * @brief Função de escrita binária.
   */
  void write(std::ofstream& ofs) const;

  /*!
   * @brief Função de leitura binária.
   */
  void read(std::ifstream& ifs);
  
};


/// Classe de dados gerais do jogo.
class SudokuData {
  public: 
  std::vector <std::shared_ptr<SudokuBoard>> boards; 
   
  int current_board;
  std::string message;
  /*!
   * @brief Leitura do arquivo selecionado pelo jogador (ou encontrado na pasta).
   * @param filename Nome do arquivo a ser lido.
   */

  void readFile(std::string filename);

  /*!
   * @brief Coloca a jogada informada pelo jogador.
   * @param row Linha da jogada.
   * @param col Coluna da jogada.
   * @param value Valor a ser colocado na coodenada selecionada.
   */
  void print_board(size_t k) const;

  ///@brief Construtor padrão que inicializa os dados do jogo.
  SudokuData();
};

