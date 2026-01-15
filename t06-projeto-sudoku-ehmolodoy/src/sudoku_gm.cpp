#include "sudoku_gm.hpp"
#include <cctype>
#include <iostream>
#include <string>
#include "../lib/tcolor.hpp"
#include "arguments.hpp"
#include "board.hpp"
#include <limits>

using byte = char;

using history_item_t = std::pair<std::pair<int, int>, int>;

// Função auxiliar para gerar o nome do arquivo de save baseado no índice do puzzle
std::string get_indexed_save_filename(int index) {
    return "save_puzzle_" + std::to_string(index) + ".sk";
}

SudokuGame::SudokuGame() {
  m_errors = 0;
  inGaming = false;
  m_checks_left = 3; 
  m_check_mode = false;
  m_last_move = {-1, -1};

   helper = "Commands syntax:\n"
                       "  'enter' (without typing anything)  -> go back to previous menu\n"
                       "  'p' <row> <col> <number> + 'enter' -> place <number> on board at location (<row>, <col>).\n"
                       "  'r' <row> <col>  + 'enter'         -> remove number on board at location (<row>, <col>).\n"
                       "  'c' + 'enter'                      -> check which moves made are correct.\n"
                       "  'u' + 'enter'                      -> undo last play.\n"
                       "  's' + 'enter'                      -> save the current game.\n"
                       "  <row> is in the range [A-I], <col> and <number> are [1-9].\n";
}

SudokuData::SudokuData(){
  //inicializa o indice para o primeiro tabuleiro
   current_board = 0;
   message = " ";
  
}


void SudokuGame::usage(std::string msg) const { 
 // exit(not msg.empty() ? 1 : 0); 

  std::cout << "MSG: [" << msg << "]\n";
}

bool SudokuGame::game_over() {
  return false;  
}

bool SudokuGame::save_game(){
    //  Determina o nome do arquivo baseado no índice atual.
     int index = m_sudoku_data.current_board;
    std::string filename = get_indexed_save_filename(index);
    
   std::ofstream ofs { filename, std::ios::binary};
   if(!ofs) {
       std::cerr << "File can not be saved.";
       return false;
   }

  // Serializa o tabuleiro jogado.
   m_working_board.write(ofs);

  // Serializando variaveis primitivas.
   ofs.write((byte*)&m_checks_left, sizeof(m_checks_left));
   ofs.write((byte*)&m_check_mode, sizeof(m_check_mode));
   ofs.write((byte*)&m_last_move, sizeof(m_last_move));


   // Serializando o histórico.
   size_t history_size = undo_history.size();
   ofs.write((byte*)&history_size, sizeof(size_t));

   // Salva o conteúdo do histórico.
  if(history_size > 0){
    ofs.write((byte*)undo_history.data(), history_size * sizeof(history_item_t));
  }

  ofs.close();
  return true; 

}

bool SudokuGame::load_game(){
  //  Determina o nome do arquivo baseado no índice atual.
    int index = m_sudoku_data.current_board;
    std::string filename = get_indexed_save_filename(index);

    std::ifstream ifs {filename,  std::ios::binary};

  if(!ifs.is_open() || ifs.peek() == std::ifstream::traits_type::eof() ){
     std::cerr << "FILE CAN NOT BE OPENED OR DOESNT EXIST." << "\n\n";
     return false;
  }
 
  // Desserializa o tabuleiro.
  m_working_board.read(ifs);

  // Desserializa as variáveis do jogo.
   ifs.read((byte*)&m_checks_left, sizeof(m_checks_left));
   ifs.read((byte*)&m_check_mode, sizeof(m_check_mode));
   ifs.read((byte*)&m_last_move, sizeof(m_last_move));


    size_t history_size = 0;
    ifs.read((byte*)&history_size, sizeof(size_t));
    
    //  Redimensiona o vetor e lê o conteúdo.
    if (history_size > 0) {
        undo_history.resize(history_size);
        // Garante que o resize funcionou e o vetor não está vazio antes de tentar ler.
        if (undo_history.data() != nullptr) {
            ifs.read((byte*)undo_history.data(), history_size * sizeof(history_item_t));
        } else {
             // Tratamento de erro se o resize falhar, embora improvável
             std::cerr << "ERRO: RESIZE FAIL.\n";
             return false;
        }
    } else {
        undo_history.clear();
    }

    // Verifica se a leitura falhou em algum momento (por exemplo, arquivo corrompido)
    if (ifs.fail()) {
        std::cerr << "ERROR: FILE READING FAILED.\n";
        return false;
    }

    ifs.close();
    
    // Sucesso, define a mensagem e transiciona para o estado de jogo
    m_game_state = game_state_e::PLAYING; 
    return true;
}


void SudokuGame::readCommand(SudokuBoard& board, std::string arg) {
    std::stringstream ss(arg);
    char cmd;//comando
    ss >> cmd;
    m_sudoku_data.message = ""; //clear msg

    //PLAY
    if (cmd == 'p') {
      error_mode = false;
        char row_letter;

        int r, c, v;
        if (ss >> row_letter >> c >> v) {
           r = std::toupper(row_letter) - 'A';
            c--;// 1-9 -> 0-8
            if (r < 0 || r > 8 || c < 0 || c > 8 || v < 1 || v > 9) {
                m_sudoku_data.message = "Invalid coordinates or value!"; return;
            } else if (board.is_fixed(r, c)) {
                m_sudoku_data.message = "Error: Fixed Cell!"; return;
            } else if(!m_working_board.is_safe(r, c, v)){
                  error_mode = true;
                 ++m_errors;
               if(m_errors > 3){
               
                m_game_state = game_state_e::GAMEOVER;
               } else {
                 m_sudoku_data.message = "Wrong move, you can continue if you wish!";
               }
               
            } else {
              m_sudoku_data.message = "Recorded play.";
            }
            undo_history.push_back({{r, c}, board.m_board[r][c]});//salva o que tinha antes
            
            board.set_cell(r, c, v);
            
            m_last_move = {r, c};
          
        }
    } 

    //CHECK
    else if (cmd == 'c') {
        if (m_checks_left > 0) {
            m_checks_left--;
            m_check_mode = true; //ativa cores
            m_sudoku_data.message = "Verification completed! (" + std::to_string(m_checks_left) + " remaining)";
        } else {
            m_sudoku_data.message = "No checks remaining!";
        }
    }
    //UNDO
    else if (cmd == 'u') {
if (undo_history.empty()) {
            m_sudoku_data.message = "Nothing to undo!.";
            return;
        }

        auto last_action = undo_history.back();
        int target_r = last_action.first.first;
        int target_c = last_action.first.second;
        int old_value = last_action.second;// valor q estava la

        undo_history.pop_back();

        //busca reversa no vector undo, funciona como uma pilha
        board.set_cell(target_r, target_c, old_value);


        m_last_move = {target_r, target_c}; 
        
        char row_char = 'A' + target_r;
        m_sudoku_data.message = "Undoed move in (" + std::string(1, row_char) + "," + std::to_string(target_c+1) + ")";
    }
    //REMOVE
    else if (cmd == 'r') {
      char row_letter;
      int r, c;

        if(ss >> row_letter >> c) {
          r = std::toupper(row_letter) - 'A';
          c--;
            if (r < 0 || r > 8 || c < 0 || c > 8) {
                m_sudoku_data.message = "Invalid coordinates!"; 
                return;
            }
            if (board.is_fixed(r, c)) {
                m_sudoku_data.message = "Error: Fixed cell!";
                return;
            }
            if (board.m_board[r][c] <= 0) {
                m_sudoku_data.message = "Cell is already empty!";
                return;
            }
            undo_history.push_back({{r,c}, board.m_board[r][c]});//salva o que tinha antes
            board.set_cell(r, c, 0);
            m_last_move = {r,c};
            m_sudoku_data.message = "Removed.";
        } else {
            m_sudoku_data.message = "Invalid syntax. Use: r <row> <col>";
        }
    //SAVE
    } else if (cmd == 's'){
      m_game_state = game_state_e::SAVING;
    }
}

void SudokuGame::process_events() {
  if (m_game_state == game_state_e::WELCOME){
    std::cout << '\n'
              << "==================================================\n"
              << "  Welcome to a terminal version of Sudoku, v1.0   \n"
              << " Copyrigth (C) 2025, Guilherme Melo e Lucas Noid  \n"
              << "=================================================="
              << "\n\n"
              << "Press enter to start\n";

      std::cin.get();
      m_game_state = game_state_e::STARTING;
    
  } else if (m_game_state == game_state_e::STARTING) {
    

     int option;
    std::cout << "\n1-Play 2-New Game 3-Load Saved Game 4-Quit 5-Help";
    std::cout << "\nSelect option [1,5] >  ";
    std::cin >> option;

   if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta a entrada inválida.
        m_sudoku_data.message = "Invalid input! Please enter a number [1-5]";
        m_game_state = game_state_e::STARTING;
        return; // Sai do process_events para renderizar a mensagem e tentar novamente.
    }


    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //limpa o buffer
    std::cout << '\n';

    if(option == 1){
      m_sudoku_data.message = " ";
      if(m_sudoku_data.boards.size() > 0){ 
        //copia o tabuleiro
        m_working_board = *m_sudoku_data.boards[m_sudoku_data.current_board];
        //estado
        m_game_state = game_state_e::PLAYING;
        inGaming = true;
        m_checks_left = 3; 
        m_errors = 0;
        m_last_move = {-1, -1};
        undo_history.clear();
        m_check_mode = false;
        error_mode = false;
        std::cout << "Game started... Good game!.\n";
      } else{
        m_sudoku_data.message = "There are no game boards to play on!";
        }
    }else if(option == 2){
     m_sudoku_data.message = " ";
      //atualiza o tabuleiro a ser mostrado de forma circular
    m_sudoku_data.current_board = (m_sudoku_data.current_board+1)%m_sudoku_data.boards.size();
    m_game_state = game_state_e::STARTING;

    }else if(option == 3){
      m_sudoku_data.message = " ";
      if(!load_game()){
       m_game_state = game_state_e::STARTING;
      }

    }else if(option == 4){
      if(inGaming){
        m_game_state = game_state_e::QUITTING;
      } else {
        m_game_state = game_state_e::ENDING;
      }

    }else if(option == 5){
     m_sudoku_data.message = " ";
    
       m_game_state = game_state_e::HELPING;
      
    } else{
     
      m_sudoku_data.message = "Invalid option!";
      m_game_state = game_state_e::STARTING;

    }


  } else if(m_game_state == game_state_e::HELPING) {

    
    std::cout << '\n'
              << "-------------------------------------------------------------------------------\n"
              << "  The goal of Sudoku is to fill a 9x9 grid with numbers so that each row,  \n"
              << "  column and 3x3 section (nonet) contain all of the digits between 1 and 9.\n\n"
              << "  The Sudoku rules are: "
              << "  1. Each row, column, and nonet can contain each number (typically 1 to 9)\n"
              << "  exactly once.\n"
              << "  2. The sum of all numbers in any nonet, row, or column must be equal to 45.\n"
              << "-------------------------------------------------------------------------------\n\n\n"
              << "Press enter to go back.\n";

              std::cin.get();
              m_game_state = game_state_e:: STARTING;


  } else if (m_game_state == game_state_e::PLAYING){

    if (m_check_mode) {
        std::cout << "Press Enter to continue...";
        //std::cin.ignore(); //acho q n precisa
        std::cin.get();
        m_check_mode = false;
        return;
    }

    std::string command;
    std::cout << "\n" << "Enter command > ";
    getline(std::cin, command);
    std::cout << '\n';

  if (command.empty()){ 
    m_game_state = game_state_e::STARTING;
    return;//caso enter sem nada
  } else {
    readCommand(m_working_board, command);
  }
  } else if (m_game_state == game_state_e::VICTORY) {
      std::string fallen;
      std::getline(std::cin, fallen);
      inGaming = false;//reset flag
      m_game_state = game_state_e::STARTING; 
  
  } else if (m_game_state == game_state_e::QUITTING) {
    usage("Are you sure you want leave this game?");
    char opt;
    // Lê a escolha
    std::cout << "Your choice [y/N] > ";
    std::cin >> opt;
    // Coloca o resultado para 
    opt = std::tolower(opt);
    std::cout << '\n';
    if(opt == 'y'){
      // A intenção é que se ela está em jogo e confirma que quer sair, ela sai direto
      m_game_state = game_state_e::ENDING;
    } else if(opt == 'n'){
      // Desenvolver melhor essa ideia
      // Problema: voltar ao exato momento em que o jogo parou.
      m_game_state = game_state_e::PLAYING;
    } else {
      std::cout << "Invalid option. \n\n";
      // Continua no estado enquanto não tiver uma opção válida.
      m_game_state = game_state_e::QUITTING;
    }

  } else if(m_game_state == game_state_e::ENDING){
    
    // Estado para quando estiver no menu principal (jogo não iniciado).
    // Também aparece após o quitting.
    std::cout << "Thank you for playing Sudoku!  \n";
    std::cout << "Ass: Eh molodoy Team.\n";
    exit(1);

  } else if(m_game_state == game_state_e::GAMEOVER){
    std::cout << "\nYou lost! Press enter to continue\n\n";
     std::cin.get();
     m_sudoku_data.message = "";
     m_game_state = game_state_e::STARTING;
  }
}

void SudokuGame::setup(const RunningOptions& run_opt) {

  // Define o primeiro estado como Welcome. 
  m_game_state = game_state_e::WELCOME;
  // Cria um novo banco de dados.
  
  // Lê o arquivo. 
  m_sudoku_data.readFile(run_opt.filename);

}

void SudokuGame::update() {
   if(m_game_state == game_state_e::SAVING){
      
    save_game();
    m_game_state = game_state_e::STARTING;
   }
   else if (m_game_state == game_state_e::PLAYING) {
  
    inGaming = true;
    const SudokuBoard* original = m_sudoku_data.boards[m_sudoku_data.current_board].get();
      if (m_working_board.solved(*original)) {
          m_game_state = game_state_e::VICTORY;
          m_sudoku_data.message = "CONGRATULATIONS! You solved the puzzle! Press [ENTER] to exit."; //msg de vitoria
      }
    }
}

void SudokuGame::render() const {
  if(m_game_state == game_state_e::STARTING){
    
    std::cout  << TColor::colorize("|-------[ MAIN SCREEN ]-------|\n\n", TColor::CYAN);
    // Mostra o tabuleiro de acordo com a numeração do tabuleiro atual. 
    m_sudoku_data.print_board(m_sudoku_data.current_board);
    usage(m_sudoku_data.message);
    
  } else if (m_game_state == game_state_e::PLAYING || m_game_state == game_state_e::VICTORY){

     std::cout << "\n";//melhorar visualização
     std::cout  << TColor::colorize("|--------[ PLAY MODE ]--------|\n\n", TColor::CYAN);

     const SudokuBoard* original = m_sudoku_data.boards[m_sudoku_data.current_board].get();//tabuleiro original
     m_working_board.print(original, m_check_mode, m_last_move, error_mode);

     //contagem de dígitos
     int counts[10] = {0};
     for(int i=0; i<9; ++i)
        for(int j=0; j<9; ++j)
            if(m_working_board.m_board[i][j] > 0)
                counts[m_working_board.m_board[i][j]]++;
     
     std::cout <<   TColor::colorize("Checks left: ", TColor::YELLOW) << m_checks_left << "\n";
     std::cout  << TColor::colorize("Digits left: ", TColor::BRIGHT_YELLOW);
     for(int k=1; k<=9; ++k) {
         int falta = 9 - counts[k];
         if(falta > 0) std::cout << k << ":(" << falta << ") ";
     }
     std::cout << "\n";
     std::cout <<   TColor::colorize("Errors: ", TColor::YELLOW) << m_errors << "\n";
     std::cout << "\n\n";

    if (m_game_state == game_state_e::VICTORY) {
         std::cout << "MSG: [ " << TColor::colorize(m_sudoku_data.message, TColor::BRIGHT_GREEN) << " ]\n";
    } else {
         std::cout << "MSG: [ " << TColor::colorize(m_sudoku_data.message, TColor::YELLOW) << " ]\n";
      }

     if (!m_check_mode) { std::cout  << TColor::colorize(helper, TColor::BRIGHT_BLUE);}
  }
  }
