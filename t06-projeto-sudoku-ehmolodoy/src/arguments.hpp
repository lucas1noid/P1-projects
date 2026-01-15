#pragma once

#include <string>

/*!
 * @brief Struct para guardar a quantidade de Checks e o nome do arquivo.
 */
struct RunningOptions {
  int checks{3};
  std::string filename{};
};
/*!
 * @brief Retorna um objeto de Running Options preechido com os argumentos
 * passados via linha de comando (CLI).
 * @param argc Quantidade de argumentos.
 * @param argv Os argumentos em si.
 */
RunningOptions parse_cli_arguments(int argc, char** argv);
