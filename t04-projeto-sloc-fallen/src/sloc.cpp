#include <fstream>
#include <iostream>
#include <string>
#include "funcs.hpp"
#include "sloc_types.hpp"

#include <filesystem>  // leitura de diretorios recursivamente
#include <fstream>     // For reading files
#include <iostream>    // For error messages
#include <string>      // For std::string
#include "funcs.hpp"   // For the function declaration and trim()

/*!
 *
 * Scans a directory for C/C++ source files, optionally recursively.
 *
 * @param caminhoDir The path to the directory to scan.
 * @param options The running_options, used to determine if the scan should be recursive.
 *
 * @return A vector of FileInfo structs, one for each valid source file found.
 *
 */
std::vector<FileInfo> processar_diretorios(const std::string& caminhoDir,
                                           const RunningOpt& options) {
  std::vector<FileInfo> resultados;
  std::filesystem::path diretorioPath(caminhoDir);

  // o iterator usado depende da opção -r
  if (options.recursive) {
    // usa o recursive_directory_iterator
    for (const auto& entry : std::filesystem::recursive_directory_iterator(diretorioPath)) {
      if (entry.is_regular_file()) {
        std::filesystem::path caminho = entry.path();
        std::string ext = caminho.extension().string();  // .extension() retorna com o ponto

        if (ext == ".cpp" || ext == ".hpp" || ext == ".c" || ext == ".h") {
          resultados.push_back(sloc(caminho.string()));
        }
      }
    }
  } else {
    // usa o directory_iterator normal para varrer apenas o diretorio atual
    for (const auto& entry : std::filesystem::directory_iterator(diretorioPath)) {
      if (entry.is_regular_file()) {
        std::filesystem::path caminho = entry.path();
        std::string ext = caminho.extension().string();

        if (ext == ".cpp" || ext == ".hpp" || ext == ".c" || ext == ".h") {
          resultados.push_back(sloc(caminho.string()));
        }
      }
    }
  }
  return resultados;
}

/*!
 *
 * Counts the lines of code, comments, and blank lines in a source file.
 *
 * @param filename The full path to the file to be analyzed.
 *
 * @return A FileInfo containing the counted metrics for the file.
 */
FileInfo sloc(const std::string& nomeDoArquivo) {
  FileInfo result;
  result.filename = nomeDoArquivo;

  // armazenar os tipos para conseguir fazer a ordenacao baseada em 't'
  std::string tipo = nomeDoArquivo.substr(nomeDoArquivo.find_last_of(".") + 1);
  if (tipo == "cpp") {
    result.type = CPP;
  } else if (tipo == "hpp") {
    result.type = HPP;
  } else if (tipo == "c") {
    result.type = C;
  } else if (tipo == "h") {
    result.type = H;
  } else {
    result.type = UNDEF;
  }

  state_e currentState = NONE;

  std::ifstream file(nomeDoArquivo);
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Error: Não é possível abrir o arquivo '" << nomeDoArquivo << "'\n";
    return result;
  }

  while (std::getline(file, line)) {
    result.n_lines++;

    if (trim(line).empty()) {
      // caso esteja dentro de um bloco
      if (currentState == COMMENT_BLOCK || currentState == BLOC) {
        result.n_comments++;  // uma linha de comentario em branco
      } else if (currentState == DOXY_BLOCK) {
        result.n_doxy++;  // uma linha de doxy em branco
      } else {            // caso n seja um bloco é apenas uma linha em branco normal
        result.n_blank++;
      }
      // com a linha vazia, podemos pular para a proxima
      continue;
    }

    bool line_has_loc = false;
    bool line_has_regular_comment = false;
    bool line_has_dox_comment = false;

    //  se começar a linha dentro de um bloco, a linha será sempre um comentário, doxy ou comum.

    for (char const& n : line) {
      switch (currentState) {
      case NONE:
        if (n == '/') {
          currentState = POSSIBLE_COMMENT;
        } else if (n == '"') {  // inicia uma string
          currentState = STRING;
          line_has_loc = true;
        } else if (n == '\'') {  // inicia um char literal
          currentState = CHAR_LITERAL;
          line_has_loc = true;
        } else if (n > ' ') {
          currentState = CODE;
          line_has_loc = true;
        }
        break;
      case CODE:
        if (n == '"') {
          currentState = STRING;
        } else if (n == '\'') {  // agr lida com char literal
          currentState = CHAR_LITERAL;
        } else if (n == '/') {
          currentState = POSSIBLE_COMMENT;
        } else if (n > ' ') {
          line_has_loc = true;
        }
        break;
      case STRING:
        if (n == '\\') {  // encontrou uma barra de escape
          currentState = ESCAPE_SEQUENCE;
        } else if (n == '"') {
          currentState = CODE;
        }
        break;
      case CHAR_LITERAL:
        // simplificação: esta versão não lida com escapes dentro de chars (ex: '\''),
        // mas já resolve 99% dos casos, como char c = '/';
        if (n == '\'') {
          currentState = CODE;
        }
        // se não for aspas simples continua char literal
        break;
      case ESCAPE_SEQUENCE:
        // nao importa qual é o caractere após a barra, apenas consumimos ele e voltamos para o
        // estado de string
        currentState = STRING;
        break;
      case POSSIBLE_COMMENT:
        if (n == '/') {
          currentState = LINE;
        } else if (n == '*') {
          currentState = BLOC;
          line_has_regular_comment = true;
        } else {
          currentState = CODE;
        }
        break;
      case BLOC:
        if (n == '*' || n == '!') {
          currentState = DOXY_BLOCK;
          line_has_dox_comment = true;
          line_has_regular_comment = false;
        } else {
          currentState = COMMENT_BLOCK;
          line_has_regular_comment = true;
        }
        break;
      // mantem
      case LINE:
        if (n == '/' || n == '!') {
          currentState = DOXY_LINE;
        } else {
          currentState = COMMENT_LINE;
        }
        break;
      case DOXY_LINE:
        line_has_dox_comment = true;
        break;
      case COMMENT_LINE:
        line_has_regular_comment = true;
        break;
      case DOXY_BLOCK:
        line_has_dox_comment = true;
        if (n == '*') {
          currentState = END_DOXY_BLOC;
        }
        break;
      case END_DOXY_BLOC:
        if (n == '/') {
          currentState = NONE;
          line_has_dox_comment = true;
        } else {
          currentState = DOXY_BLOCK;
        }
        break;
      case COMMENT_BLOCK:
        line_has_regular_comment = true;
        if (n == '*')
          currentState = END_COM_BLOC;
        break;
      case END_COM_BLOC:
        if (n == '/') {
          currentState = NONE;
          line_has_regular_comment = true;
        } else {
          currentState = COMMENT_BLOCK;
        }
        break;
      }
    }

    // atualiza o total baseado no que ele encontra na linha
    if (line_has_loc)
      result.n_loc++;
    if (line_has_regular_comment)
      result.n_comments++;
    if (line_has_dox_comment)
      result.n_doxy++;

    if (currentState != BLOC && currentState != COMMENT_BLOCK && currentState != END_COM_BLOC
        && currentState != END_DOXY_BLOC && currentState != DOXY_BLOCK) {
      currentState = NONE;
    }
  }

  return result;
}
