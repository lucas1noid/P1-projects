#include "arguments.hpp"
#include <cctype>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

bool isInteger(const std::string s){
  
 if (s.empty()) return false;
    
    size_t start = 0;
    // Permite um sinal de menos opcional no início
    if (s[0] == '-') {
        start = 1;
    }

    // Se a string for apenas "-" ou vazia, ela não é um inteiro válido.
    if (start == s.length()) {
        return false;
    }
    
    for(size_t i = start; i < s.length(); ++i){
        // Verifica se os caracteres restantes são dígitos.
        if(!std::isdigit(s[i])){
            return false;
        }
    }

    try{
        // Tenta converter para verificar se não há overflow (caso a string seja muito longa).
        std::stoi(s);
        return true;
    } catch(...){
        return false;
    }
}

void help (std::string msg, const int usg)  {
   if(!msg.empty()){
    std::cout << msg << "\n\n";

   }

   if(usg == 1){
      std::string helper = "Usage: sudoku [-c <num>] [-h] <input_puzzle_file> \n"
                        "  Game options: \n"
                        "    -c <num> Number of checks per game. Default = 3. \n"
                        "    -h Print this help text.\n";

       std::cout << helper << '\n';
    }
}

bool valid_file(std::filesystem::path filename){
  
    std::string line;
    std::ifstream file(filename);
    int board_rows = 0; 
     
    while(getline(file, line)){
      if(line.empty()){
        //Verifica se o tabuleiro anterior estava completo
        if(board_rows > 0 && board_rows != 9){
          return false;
        }
        board_rows = 0;
        continue;
      }
    
      if(board_rows >= 9){
        return false; //Tabuleiro excedeu a quantidade de linhas.
      }
      std::istringstream iss(line);
      std::string num;
      int col_count = 0; // Contador de colunas na linha.

      while(iss >> num){
        if(!isInteger(num)){
          return false; // Não é um dígito inteiro.
        }
        int val = std::stoi(num);

        if(val < -9 || val > 9){
          return false; // Inteiro fora do intervalo.
        }

        col_count++;
      }

      if(col_count != 9){
        return false; // Não tem 9 numeros na linha. 
      }

      board_rows++;
    }
    // Garante que o ultimo tabuleiro estava completo.
     if (board_rows > 0 && board_rows != 9) {
        return false;
    }
    return true; 
}
/// Returns a Running Options object filled in with the arguments passed via CLI.
RunningOptions parse_cli_arguments(int argc, char** argv) {
  RunningOptions run_opt;
  std::filesystem::path p;
  bool file_Found = false;
  // TODO: parse arguments here.
  for(auto i{0}; i < argc; ++i){
    std::string arg = argv[i];
   

    // Comando para numero de checks.
    if(arg == "-c"){
    // Verifica se o valor passado é um inteiro. 
      if(i + 1 < argc && isInteger(argv[i+1])){
        run_opt.checks = std::stoi(argv[i+1]);
        i++; // Pula o valor lido. 
       } else{
        help("Invalid value, number of Checks will be 3.", 1);
      }
    }

    // Mostra a tela de ajuda.
    if(arg == "-h" or arg == "--help"){
       help(" ",1);
    }
   
    // Verifica se é um arquivo regular.
    if(std::filesystem::is_regular_file(arg)){
      p = arg;
    // Verifica se a extensão do arquivo é .txt
      if(p.extension() == ".txt"){
       file_Found = true;
       run_opt.filename = arg;
      }
     }

  } 

  
  if(file_Found == false){
    help("No files inserted, searching in the current folder.",0);
    // Acessa a pasta atual. 
    std::filesystem::path currentDir = std::filesystem::current_path();
    
    // Verifica os arquivos da pasta atual.
    for(const auto& entry : std::filesystem::directory_iterator(currentDir)){
      if(entry.is_regular_file()){
        p = entry;
        // Verifica a extensão.
        if(p.extension() == ".txt" && valid_file(p)){
          help("File found.",0);
          run_opt.filename = p;
          file_Found = true;
          break;
        }
      }
    }
  }
  
  if(file_Found == false){
    help("No files found.", 0);
    exit(1);
  } 

  return run_opt;
}
