#ifndef SLOC_TYPES_HPP
#define SLOC_TYPES_HPP

#include <cstdint>
#include <cstdlib>
#include <dirent.h>
#include <string>
#include <utility>
#include <vector>

//== Enumerations

/// This enumeration lists all the supported languages.
enum lang_type_e : std::uint8_t {
  C = 0,  //!< C language
  CPP,    //!< C++ languagesloc -> possible comment -> linha unica
  H,      //!< C/C++ header
  HPP,    //!< C++ header
  UNDEF,  //!< Undefined type.
};

enum state_e : std::uint8_t {
  NONE = 0,  //!< em branco
  CODE,      //!< sloc
  STRING,    //!< estado auxiliar do code
  CHAR_LITERAL,//novo
  ESCAPE_SEQUENCE,//novo tbm, seu papel é basicamente ignorar oq vem depois da '\'
  POSSIBLE_COMMENT,
  LINE,
  BLOC,
  END_COM_BLOC,
  END_DOXY_BLOC,
  COMMENT_LINE,   //!< commentario
  COMMENT_BLOCK,  //!< "/* ... */"
  DOXY_LINE,      //!< "///" ou "//!"
  DOXY_BLOCK      //!< "/** ... */" ou "/*! ... */"
};

//== Class/Struct declaration

/// Integer type for counting lines.
using count_t = unsigned long;
/// Stores the file information we are collecting.
class FileInfo {
public:
  std::string filename;  //!< the filename.
  lang_type_e type;      //!< the language type.
  count_t n_blank;       //!< # of blank lines in the file.
  count_t n_doxy;
  count_t n_comments;  //!< # of comment lines.
  count_t n_loc;       //!< # lines of code.
  count_t n_lines;     //!< # of lines.

  /// Ctro.
  FileInfo(std::string fn = "",
           lang_type_e t = UNDEF,
           count_t nb = 0,
           count_t nc = 0,
           count_t nl = 0,
           count_t ni = 0,
           count_t nd = 0)
      : filename{ std::move(fn) }, type{ t }, n_blank{ nb }, n_doxy{ nd }, n_comments{ nc },
        n_loc{ nl }, n_lines{ ni } {}
};

/// The running options provided via CLI.
struct RunningOpt {
  std::vector<std::string> input_list;  //!< This might be a list of filenames or a directories.
  bool help = false;
  bool recursive = false;
  char sortkey = '\0';       // chave de ordenação
  bool sort_decres = false;  // false = "-s" e true = "-S"
};

#endif  // !SLOC_TYPES_HPP
