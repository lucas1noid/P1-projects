/*!
 * @file main.cpp
 * @description
 * This program implements a single line of code count for C/C++ programs.
 * @author   Samuel Jacome e Lucas Lopes
 * @date   September, 9th 2024.
 * @remark On 2022-09-9 changed ...
 */

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "funcs.hpp"
#include "sloc_types.hpp"

int main(int argc, char* argv[]) {

  // cria um options com todos os argumentos
  RunningOpt options;
  validate_args(argc, argv, options);

  if (options.help || argc == 1) {
    print_help();
    return EXIT_SUCCESS;
  }

  std::vector<FileInfo> db;

  for (const auto& entrada_str : options.input_list) {
    std::filesystem::path caminho(entrada_str);  // converte a string para um objeto path

    if (!std::filesystem::exists(caminho)) {  // caso não exista
      std::cerr << "Erro: O caminho '" << entrada_str << "' nao existe.\n";
      continue;
    }

    if (std::filesystem::is_directory(
          caminho)) {  // caso seja diretorio, chama a processar_diretorios
      auto resultadosDir = processar_diretorios(entrada_str, options);
      // adiciona todos os FileInfo retornados ao nosso db
      db.insert(db.end(), resultadosDir.begin(), resultadosDir.end());

    } else if (std::filesystem::is_regular_file(caminho)) {  // caso arquivo, chama sloc
      db.push_back(sloc(entrada_str));
    }
  }

  // ordenando o cout
  if (options.sortkey != '\0') {
    // Uma função lambda que define como comparar dois FileInfo
    auto comparador = [&](const FileInfo& a, const FileInfo& b) {
      bool resultado;
      switch (options.sortkey) {
      case 'f':  // Filename
        resultado = a.filename < b.filename;
        break;
      case 't':  // Filetype
        resultado = a.type < b.type;
        break;
      case 'a':  // Total lines
        resultado = a.n_lines < b.n_lines;
        break;
      case 'c':  // Comments
        resultado = a.n_comments < b.n_comments;
        break;
      case 'd':  // Doxygen
        resultado = a.n_doxy < b.n_doxy;
        break;
      case 'b':  // Blank lines
        resultado = a.n_blank < b.n_blank;
        break;
      case 's':  // Sloc lines
        resultado = a.n_loc < b.n_loc;
        break;
      default:
        // caso a chave for invalida (no caso do validate falhar)
        return false;
      }

      // se for -S (descendente), inverte o resultado da comparação
      return options.sort_decres ? !resultado : resultado;
    };

    std::sort(db.begin(), db.end(), comparador);  // std::sort para ordenar o vetor
  }

  // agora printando na estrutura que o professor pediu
  print_tabela(db);

  return EXIT_SUCCESS;
}
