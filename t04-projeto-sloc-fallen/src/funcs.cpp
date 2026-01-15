#include "funcs.hpp"  // Conecta este arquivo ao seu cabeçalho

// Bibliotecas necessárias para as funções abaixo
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//================================================================================
// IMPLEMENTAÇÕES
//================================================================================

/*!
 *
 * This Funtion takes a language a language type enum value and returns the
 * corresponding string representation of the progamming language
 *
 *
 * @param tipo The language type enumeration value to convert
 * @return The string representation of the language type
 *
 * @note If an invalid or unrecognized type is passed, the function returns "Undefined"
 */
std::string tipo_to_string(lang_type_e tipo) {
  switch (tipo) {
  case C:
    return "C";
  case CPP:
    return "C++";
  case H:
    return "H";
  case HPP:
    return "HPP";
  default:
    return "Undefined";
  }
}

/*!
 * * Function that prints the quantities of each file passed by the program
 * * @param info All printable information for each file.
 * * @return Its a void function.
 * * @note !!PRECISA SER ALTERADA!!
 */
void print_tabela(const std::vector<FileInfo>& db) {
  if (db.empty()) {
    std::cout << "Nenhum arquivo de código-fonte válido encontrado para análise.\n";
    return;
  }

  // calcular larguras e totais
  size_t max_len = 12;  // "Filename"
  for (const auto& info : db) {
    if (info.filename.length() > max_len) {
      max_len = info.filename.length();
    }
  }
  max_len += 2;  // para garantir espaco entre a coluna filename e a language

  FileInfo sum;  // um fileinfo extra para guardar as somas
  for (const auto& info : db) {
    sum.n_comments += info.n_comments;
    sum.n_doxy += info.n_doxy;
    sum.n_blank += info.n_blank;
    sum.n_loc += info.n_loc;
    sum.n_lines += info.n_lines;
  }

  // define as linhas e as dimensoes da tabela
  const int lang_w = 20;
  const int count_w = 18;
  const std::string separator(max_len + lang_w + count_w * 5 + 10, '-');

  // cabeçalho
  std::cout << "Files processed: " << db.size() << "\n";  // num de files
  std::cout << separator << "\n";
  std::cout << std::left << std::setw(max_len) << "Filename" << std::setw(lang_w) << "Language"
            << std::right << std::setw(count_w) << "Comments" << std::setw(count_w)
            << "Doc Comments" << std::setw(count_w) << "Blank" << std::setw(count_w) << "Code"
            << std::setw(count_w) << "# of lines"
            << "\n";
  std::cout << separator << "\n";

  // imprimi as linhas de dados
  for (const auto& info : db) {
    /// formata e retorna o numero ja com a porcentagem com uma funçao lambda para evitar repetir
    /// código dentro da funcao principal
    auto format_count = [&](count_t count, count_t total) {
      std::stringstream ss;
      double percentage = (total == 0) ? 0 : ((double)count / total * 100.0);  // ternário
      ss << count << " (" << std::fixed << std::setprecision(2) << percentage << "%)";
      return ss.str();
    };

    std::cout << std::left << std::setw(max_len) << info.filename << std::setw(lang_w)
              << tipo_to_string(info.type)  // tipo_to_string apenas pra isso
              << std::right << std::setw(count_w) << format_count(info.n_comments, info.n_lines)
              << std::setw(count_w) << format_count(info.n_doxy, info.n_lines) << std::setw(count_w)
              << format_count(info.n_blank, info.n_lines) << std::setw(count_w)
              << format_count(info.n_loc, info.n_lines) << std::setw(count_w) << info.n_lines
              << "\n";
  }

  // rodapé com a soma
  std::cout << separator << "\n";
  std::cout << std::left << std::setw(max_len + lang_w) << "SUM" << std::right << std::setw(count_w)
            << sum.n_comments << std::setw(count_w) << sum.n_doxy << std::setw(count_w)
            << sum.n_blank << std::setw(count_w) << sum.n_loc << std::setw(count_w) << sum.n_lines
            << "\n";
  std::cout << separator << std::endl;
}

/// @brief Function that prints the help painel
void print_help() {
  std::cout
    << "Welcome to sloc cpp, version 1.0, (c) DIMAp/UFRN.\n\n"
    << "NAME\n"
    << "  sloc - single line of code counter.\n\n"
    << "SYNOPSIS\n"
    << "  sloc [-h | --help] [-r] [(-s | -S) f|t|c|b|s|a] <file | directory>\n\n"
    << "EXAMPLES\n"
    << "  sloc main.cpp sloc.cpp\n"
    << "      Counts loc, comments, blanks of the source files 'main.cpp' and 'sloc.cpp'\n\n"
    << "  sloc source\n"
    << "      Counts loc, comments, blanks of all C/C++ source files inside 'source'\n\n"
    << "  sloc -r -s c source\n"
    << "      Counts loc, comments, blanks of all C/C++ source files recursively inside 'source'\n"
    << "      and sort the result in ascending order by # of comment lines.\n\n"
    << "DESCRIPTION\n"
    << "  Sloc counts the individual number of **lines of code** (LOC), comments, and blank\n"
    << "  lines found in a list of files or directories passed as the last argument\n"
    << "  (after options).\n"
    << "  After the counting process is concluded the program prints out to the standard\n"
    << "  output a table summarizing the information gathered, by each source file and/or\n"
    << "  directory provided.\n"
    << "  It is possible to inform which fields sloc should use to sort the data by, as\n"
    << "  well as if the data should be presented in ascending/descending numeric order.\n\n"
    << "OPTIONS:\n"
    << "  -h/--help\n"
    << "            Display this information.\n\n"
    << "  -r\n"
    << "            Look for files recursively in the directory provided.\n\n"
    << "  -s f|t|c|d|b|s|a\n"
    << "            Sort table in ASCENDING order by (f)ilename, (t) filetype,\n"
    << "            (c)omments, (d)oc comments, (b)lank lines, (s)loc, or (a)ll.\n"
    << "            Default is to show files in ordem of appearance.\n\n"
    << "  -S f|t|c|d|b|s|a\n"
    << "            Sort table in DESCENDING order by (f)ilename, (t) filetype,\n"
    << "            (c)omments, (d)oc comments, (b)lank lines, (s)loc, or (a)ll.\n"
    << "            Default is to show files in ordem of appearance." << std::endl;
}
