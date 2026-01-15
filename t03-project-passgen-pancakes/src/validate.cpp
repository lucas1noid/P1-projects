#include <iostream>
#include <string>
#include <string_view>

// LIB. ADD
#include "functions.hpp"
#include "groups.hpp"

/**
 *
 * Displays the Error panel when something get wrong with the parameters.
 *
 */
void usage(std::string_view msg = "") {
  if (!msg.empty()) {
        std::cout << "Erro em: " << msg << std::endl; //mensagem de erro
    }
   std::cout << '\n'
         << " Como usar o programa:\n"
         << "  - este programa cria uma senha de caracteres de acordo com os parâmetros digitados após o executável.\n"
         << "  - sintaxe: $ ./passgen [<opções>]\n"
         << " caso não lembre dos comandos, utilize:\n"
         << "  -h | --help  exibe uma tela de ajuda, la você irá encontrar todas as opções de como modificar sua senha corretamente.\n\n"
         << " Se foi apenas um erro de digitação acidental, o corrija e tente novamente...\n"
         << " atenciosamente, Lucas e André." << std::endl;
}

/*
 *
 * Displays the help panel when requested by the user. 
 *
 */
void help_doit(){
std::cout << " -- Welcome to passgen v 1.0, © DIMAp 2025 --\n\n"
         << "Este programa gera senhas de comprimentos específicos\n"
         << "baseado na seleção de grupos de caracteres do usuário.\n\n"
         << "Uso: $ ./passgen [<opções>]\n"
         << "  --len n            Tamanho da senha (por padrão será 7 caso não informado).\n"
         << "  -l, --lower        Adiciona caracteres aleatórios de [a-z].\n"
         << "  -u, --upper        Adiciona caracteres aleatórios de [A-Z].\n"
         << "  -d, --digits       Adiciona caracteres aleatórios de [0-9].\n"
         << "  -o, --logograms    Adiciona caracteres aleatórios de [#$%&@^`~].\n"
         << "  -p, --punctuation  Adiciona caracteres aleatórios de [.,;:].\n"
         << "  -q, --quotes       Adiciona caracteres aleatórios de [\"'].\n"
         << "  -s, --slashes      Adiciona caracteres aleatórios de [\\/|_-].\n"
         << "  -m, --math         Adiciona caracteres aleatórios de [*+!?=].\n"
         << "  -b, --braces       Adiciona caracteres aleatórios de [{}[]()].\n"
         << "  -a, --all-groups   Adiciona caracteres aleatórios de todos os grupos acima automaticamente.\n"
         << "  -v, --verbose      Imprime informações adicionais da senha além da mesma.\n"
         << "  -h, --help         Mostra essa tela de ajuda." << std::endl;
}
  
/*!
 * Function that reads and stores password parameters requested by the user..
 * 
 * @param argc Number of parameters and the name of the function..
 * @param argv Name of the program and array of chars for each parameter.
 * @param run_options Will receive all the necessary data to create the password, such as length,
 * appropriate groups and extra information.
 *
 * @return void function.
 * 
 */
void validate_arguments(int argc, char* argv[], RunningOptions& run_options) {

  

  if (argc == 1 ) {
    run_options.error = true;
    help_doit();//call the help
    return;
  }
  if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
    run_options.error = true;
    help_doit();//call the help
        return;
    }

  for (int count = 1; count < argc; ++count) {
    std::string option {argv[count]};
    
    if (option == "--len") {
      if(argc == 2) {
        usage("Tamanho da senha invalido");
        run_options.error = true;
        return;
      }
      ++count;
      option = argv[count];
    
      for (char c : option) {
        if (!(c >= '0' and c <= '9')) {
          run_options.error = true;
          usage("Tamanho da senha invalido");
          return;
        }
      }
      run_options.pass_length = stoi(option);
      if(argc == 3) {
        run_options.groups["lower"].second = true;
        return;
      }
      continue;
    }

    if (option == "--verbose") {
          run_options.show_password_strength = true;
          continue;
        } 
    if (option == "--all-groups") {
          for (auto itr = run_options.groups.begin(); itr != run_options.groups.end(); ++itr){
            if (itr -> second.second == true) {
            continue;
            }
            run_options.groups_chosen++;
            itr -> second.second = true;
            }
      continue;
        }

    //EXTENSE COMMAND
    if (option[0] == '-' and option[1] == '-') { 
    auto itr{run_options.groups.find(option.substr(2))}; //THIS FUNCTION WILL RETURN A INTERATOR TO THE LOCAL OF option OR A ITR TO THE END OF MAP
      if(itr != run_options.groups.end()) {
        if (itr -> second.second == true) {
          continue;
          } 
        itr -> second.second = true;                      // IF THE ITR ISNT THE END OF DICTIONARY, IT FOUND THE option (itr -> second.second 
        ++run_options.groups_chosen;
        continue;}                                          // is for acess the second element of dictionary and the second element of pair) 
      else {
        run_options.error = true;
        usage (option);
        return;
      } 
    }
    //REDUCED COMMAND
    else if (option[0] == '-') { 
      for (char c : option.substr(1)) {
        if (c == 'a') {
          for (auto itr = run_options.groups.begin(); itr != run_options.groups.end(); ++itr){
          if (itr -> second.second == true) {
          continue;
          }
          run_options.groups_chosen++;
          itr -> second.second = true;
          }
          continue;
        }
        if (c == 'v') {
          run_options.show_password_strength = true;
          continue;
        }
         if (c == 'h' ) {
          run_options.error = true;
          help_doit();//call the help
          return;
        }
        auto itr{run_options.char_to_string.find(c)};
        if (itr != run_options.char_to_string.end()) {
          if (run_options.groups[itr -> second].second == true) {
          continue;
          } 
          run_options.groups[itr -> second].second = true; //SEE HOW TO ACESS THIS ARGUMENT WITH THE STRING GIVED
          ++run_options.groups_chosen; 
        }
        else {
          run_options.error = true;
          usage(option);
          return;
          }
        }
      }
    }
  }


