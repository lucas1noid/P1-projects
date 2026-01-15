#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <algorithm>
#include <set>

#include "groups.hpp"
#include "functions.hpp"

/// Validates and parses the command line arguments

/*!
 * Function that return a number between [0 and n].
 * 
 * @param first First number of range.
 * @param last Last number of range. 
 * 
 * @return pseudo-random int between [0,n]. 
 *
 * @note Is an auxiliary function to the function: 'generate_password'.
 */
int die (int first = 0, int last = 0) {
  static std::random_device rd; 
  static std::mt19937 mt(rd());
  std::uniform_int_distribution <int> dist (first,last);
  return dist(mt); 
} 

/*!
 * Generates a random password based on specified preferences.
 * This function constructs a password by ensuring that, if requested, at least one character
 * from each set (uppercase, lowercase, digits, ...) is included.
 * 
 * @param run_options Has all the necessary data to create the password, such as length, 
 * appropriate groups and extra information.
 * 
 * @return The password with all the parameters requested by the user (with the exception of passwords shorter than the number of selected groups).
 * 
 * @note The password quality depends almost purely on user inputs.
 */
std::string generate_password (const RunningOptions& run_options) {
    std::vector<std::string> true_groups;
    std::string all_chars = "";
    for (const auto& pair : run_options.groups) {
        if (pair.second.second) { // se o grupo está ativo
            true_groups.push_back(pair.second.first);
            all_chars += pair.second.first;
        }
    }
    
    std::string password = ""; // senha
    std::set<char> used_chars; // guarda os caracteres já selecionados para a base tamanha > groups
    
    static std::random_device rd;
    static std::mt19937 g(rd());
    // embaralha os grupos para o caso de pass_length < numero de grupos
    std::shuffle(true_groups.begin(), true_groups.end(), g);

    //garantir 1 caractere de cada grupo (a base da senha)
    for (const auto& group : true_groups) {
        if (password.length() >= run_options.pass_length) {
        break; // para caso a senha já tenha atingido o tamanho
        }

        int random_index = die(0, group.length() - 1);
        char selected_char = group[random_index];

        password += selected_char;
        used_chars.insert(selected_char);
    }
    
    //preencher com os caracteres únicos restantes
    if (password.length() < run_options.pass_length) {
        std::string remaining_chars = "";
        for (char c : all_chars) {
            if (used_chars.find(c) == used_chars.end()) { //se o caractere NÃO foi usado na base
                remaining_chars += c;
            }
        }
        std::shuffle(remaining_chars.begin(), remaining_chars.end(), g);

        size_t space_to_fill = run_options.pass_length - password.length();
        size_t chars_to_add = std::min(space_to_fill, remaining_chars.length());//recebe o menor entre ambos
        password += remaining_chars.substr(0, chars_to_add);
    }

    //se ainda faltar, preencher com repetições aleatórias
    if (password.length() < run_options.pass_length) {
        for (auto i = password.length(); i < run_options.pass_length; ++i) {
            int random_index = die(0, all_chars.length() - 1);
            password += all_chars[random_index];
        }
    }

    std::shuffle(password.begin(), password.end(), g);

    return password;
}

