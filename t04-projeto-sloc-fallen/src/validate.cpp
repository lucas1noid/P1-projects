#include <cstdlib>
#include <dirent.h>
#include <string>
#include <vector>
#include "sloc_types.hpp"
#include <iostream>

/*!
 * 
 * Parses command-line arguments and stores a running_options struct.
 *
 * @param argc The argument count from main().
 * @param argv The argument vector from main().
 * @param options The options struct to be filled based on the parsed arguments.
 * 
 * @return the input options properly processed and stored.
 */
RunningOpt validate_args(int argc, char* argv[], RunningOpt& options) {

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      options.help = true;
    } else if (arg == "-r") {
      options.recursive = true;
    } else if (arg == "-s" || arg == "-S") {
      options.sort_decres = (arg == "-S");

      if (i + 1 < argc) {
        options.sortkey = argv[++i][0];  // pega a letra (f, t, c, ...)
      } else {
        std::cerr << "Erro: precisa especificar uma chave após " << arg << "\n";
      }
    } else {
      // qualquer outra coisa vai pra lista de arquivos/diretórios
      options.input_list.push_back(arg);
    }
  }

  return options;
}
