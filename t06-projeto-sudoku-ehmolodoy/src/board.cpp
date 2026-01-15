#include "board.hpp"
#include "../lib/tcolor.hpp"
#include "arguments.hpp"
#include "sudoku_gm.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

using byte = char;

SudokuBoard::SudokuBoard() {
// Inicializa um tabuleiro com zeros
     for (int i = 0; i < max_dim; ++i) {
          for (int j = 0; j < max_dim; ++j) {
      m_board[i][j] = 0;
      m_fixed[i][j] = false;
          }
     }
}

void SudokuBoard::write(std::ofstream& ofs) const {
   // Escreve a matriz de valores.
   ofs.write((byte*)m_board, sizeof(m_board));
   
  // Escreve a matriz de células fixas.
   ofs.write((byte*)m_fixed, sizeof(m_fixed));
}

void SudokuBoard::read(std::ifstream& ifs) {
  // Lê a matriz de valores (m_board).
   ifs.read((byte*)m_board, sizeof(m_board));

   // Lê a matriz de células fixas (m_fixed).
   ifs.read((byte*)m_fixed, sizeof(m_fixed));
}

[[nodiscard]] bool SudokuBoard::is_safe(int row, int col, value_type value) const {
  // Verifica se a linha e a coluna da entrada possuem o valor.
    for(int i = 0; i < 9; ++i){
      //Verifica a linha
      if(m_board[row][i] == value){
        return false;
      // Verifica a coluna  
      } else if (m_board[i][col] == value){
        return false;
      }
    }
    
    // Relação para decobrir a coordenada inicial do bloco.
    // Ex: row = 7 , col = 5
    // startRow = 7/3 * 3 = 2 * 3 = 6;
    // startCol = 5/3 * 3 = 1 * 3 = 3;
    int startRow = (row/3)*3;
    int startCol = (col/3)*3;
   
    // Percorre o bloco e verifica se já tem o valor.
    for(int r = startRow; r < startRow+3; ++r){
      for(int c = startCol; c < startCol+3; ++c ){
        if(m_board[r][c] == value){
          return false;
        }
      }
    }
    return true;
}

bool SudokuBoard::is_fixed(int row, int col) const {
    if (row >= 0 && row < max_dim && col >= 0 && col < max_dim) {
        return m_fixed[row][col];
    }
    return false; 
}

void SudokuBoard::set_cell(int row, int col, value_type value) {
  if (row >= 0 && row < max_dim && col >= 0 && col < max_dim) {
    m_board[row][col] = value;
  }
 
}

void SudokuBoard::remove(int row, int col) {
  if (row >= 0 && row < max_dim && col >= 0 && col < max_dim) {
     m_board[row][col] = -1;
  }
  
}

void SudokuData::print_board(size_t k) const {
  if (k >= 0 && k < boards.size()) {
        boards[k]->print();
  } else {
        std::cout << "Invalid input: " << k << "\n";
    }
}

void SudokuBoard::print(const SudokuBoard* original, bool check_mode, std::pair<int, int> last_move, bool error) const {
    //setas indicando a ultima jogada
    if (last_move.second != -1) {
        std::cout << "      "; 
        for(int j=0; j<9; ++j){
            if(j == last_move.second)  std::cout << TColor::colorize("v ", TColor::RED);
            else std::cout << "  ";
            if((j+1)%3==0) std::cout << "  ";
        }
        std::cout << "\n";
    }

    std::cout << "      1 2 3   4 5 6   7 8 9\n";
    std::cout << "    +-------+-------+-------+\n";
    
    for (int i = 0; i < max_dim; ++i) {
        
        if (i == last_move.first) std::cout << TColor::colorize("> ", TColor::RED);
        else std::cout << "  ";

        std::cout << char('A' + i) << " | "; 
        
        for (int j = 0; j < max_dim; ++j) {
            
            short val = m_board[i][j];
            
            //lógica de Renderização
            if (val <= 0) {
                std::cout << ". ";
            } else {
                if (m_fixed[i][j]) {
                    //fixos
                    std::cout << val << " ";
                } else {
                    if (check_mode && original) {
                        // Modo Check: Compara com o valor absoluto do original
                        short correto = std::abs(original->m_board[i][j]);
                        if (val == correto) {
                            std::cout << "\033[1;32m" << val << "\033[0m "; //VERDE(Acertou)
                        } else {
                            std::cout << "\033[1;31m" << val << "\033[0m "; //VERMELHO(Errou)
                        }
                    
                    
                    }
                    else if (error) {
                      // modo vermelho - erro
                         std::cout << "\033[1;31m" << val << "\033[0m ";
                    } else {
                        // modo normal:azul
                        std::cout << "\033[1;34m" << val << "\033[0m "; 
                    }
                }
            }

            if ((j + 1) % 3 == 0) std::cout << "| ";
        }
        
        if (i == last_move.first) std::cout << TColor::colorize("<", TColor::RED);

        std::cout << "\n";
        if ((i + 1) % 3 == 0) std::cout << "    +-------+-------+-------+\n";
    }

    if (last_move.second != -1) {
        std::cout << "      "; 
        for(int j=0; j<9; ++j){
            if(j == last_move.second) std::cout << TColor::colorize("^ ", TColor::RED);
            else std::cout << "  ";
            if((j+1)%3==0) std::cout << "  ";
        }
        std::cout << "\n";
    }
}

void SudokuData::readFile(std::string filename){
     // Recebe o nome do arquivo
     std::ifstream file(filename);

     if (!file.is_open()) {
        message = "Fatal error: Could not open the file. " + filename;
        return; 
    }
    boards.clear();

    std::shared_ptr<SudokuBoard> new_board = std::make_shared<SudokuBoard>();
 
     int linhaAtual{0};
     std::string line;

     while(std::getline(file, line)){
     if(line.empty()) {
      // Se for uma linha em branco, ele salva o tabuleiro e cria um novo.
      if(linhaAtual == 9){
        boards.push_back(new_board);
        // Cria um novo tabuleiro. 
        new_board = std::make_shared<SudokuBoard>();
        linhaAtual = 0;
      }
      continue;
    }
      // Modo de leitura que respeita espaços. 
      std::istringstream iss(line);
      for(int j = 0; j < 9; ++j){
        short num;
        iss >> num;

        new_board->m_board[linhaAtual][j] = num;
        
        //preencher m_fixed
        if (num > 0) {
            new_board->m_fixed[linhaAtual][j] = true;
        } else {
            new_board->m_fixed[linhaAtual][j] = false;
        }
      }
      // Atualiza a linha.
      linhaAtual++;
     }
     
     // Fecha o arquivo. 
     file.close();
     if(boards.empty()) message = "Error: No game board found in the file.";
}

bool SudokuBoard::solved(const SudokuBoard& original) const {
    for (int i = 0; i < max_dim; ++i) {
        for (int j = 0; j < max_dim; ++j) {
            if (m_board[i][j] == 0 || m_board[i][j] != std::abs(original.m_board[i][j])) {
                return false;
            }
        }
    }
    return true;
}

