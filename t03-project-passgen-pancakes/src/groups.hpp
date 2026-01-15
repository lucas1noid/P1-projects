#ifndef GROUPS__HPP
#define GROUPS__HPP

#include <string>
#include <map>

constexpr size_t default_pass_length{ 7 };

/// This struct stores the options passed in via CLI.
  


struct RunningOptions {
  size_t pass_length{ default_pass_length };  //!< Password length. tamanho!
  int groups_chosen {0};
  bool show_password_strength{false};
  bool error{false};
  
  std::map <std::string, std::pair < std::string, bool>> groups {
  {"lower",       {"abcdefghijklmnopqrstuvdwyz",  false}},
  {"upper",       {"ABCDEFGHIJKLMNOPQRSTUVWXYZ",  false}}, 
  {"digits",      {"01234566789",                 false}}, 
  {"logograms",   {"#$%&@^`~",                    false}}, 
  {"punctuation", {".,;:",                        false}}, 
  {"quotes",      {"\"'",                         false}}, 
  {"slashes",     {"\\/|_-",                      false}},
  {"math",        {"*+!?=",                       false}},
  {"braces",      {"}{][)(",                      false}},
};

  std::map <char, const std::string> char_to_string {
  {'l',"lower"}, {'u',"upper"}, {'d',"digits"}, {'o', "logograms"},
  {'p', "punctuation"}, {'q',"quotes"}, {'s', "slashes"}, {'m', "math"},
  {'b', "braces"}, {'a', "all-groups"}, {'v',"verbose"}, {'h', "help"} 
};


};

#endif
