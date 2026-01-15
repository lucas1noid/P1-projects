#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <string>
#include <vector>
#include "sloc_types.hpp"

// Declarações das funções que estarão no funcs.cpp
void print_help();
void print_tabela(const std::vector<FileInfo>& db);
std::string tipo_to_string(lang_type_e tipo);

// Funções que ainda precisam ser implementadas
RunningOpt validate_args(int argc, char* argv[], RunningOpt& options);
std::vector<FileInfo> processar_diretorios(const std::string& caminhoDir,
                                           const RunningOpt& options);
FileInfo sloc(const std::string& nomeDoArquivo);

// Definições de funções inline (precisam ficar no .hpp)
inline std::string ltrim(const std::string& s, const char* t = " \t\n\r\f\v") {
  std::string clone{ s };
  clone.erase(0, clone.find_first_not_of(t));
  return clone;
}

inline std::string rtrim(const std::string& s, const char* t = " \t\n\r\f\v") {
  std::string clone{ s };
  clone.erase(clone.find_last_not_of(t) + 1);
  return clone;
}

inline std::string trim(const std::string& s, const char* t = " \t\n\r\f\v") {
  return rtrim(ltrim(s, t), t);
}

#endif  // FUNCS_HPP
