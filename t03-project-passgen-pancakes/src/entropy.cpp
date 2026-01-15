#include <cstdlib>
#include <string_view>

// LIB. ADD
#include <cmath>
#include <set>
#include "functions.hpp"

/*!
* Function that counts how many distinct characters a string has. 
*
* @param password String to be analyzed.
* 
* @return the number of distinct characters in the strings
* 
* @note This is a auxiliary function to the function: 'password_quality'.
*/
int qtd_caracters(std::string_view password){
    
    std::set<char> unique_elements{password.begin(), password.end()};//todos os elementes presentes nao repetidos 
    
return unique_elements.size();
}

/*!
 * This function calculates and returns how "varied" and "strong" your password is, based on the entropy calculation.
 * 
 * @param password String to be analyzed.
 * 
 * @return How "strong" your password is based on a table of predefined values and the entropy value.
 * 
 * @note Entropy is not necessarily how strong the password is, but it gives an idea.
 */
std::pair <std::string,int> password_quality(std::string_view password) {
    int entropia{};// E, em bits
    int tamanho = password.size(); //tamanho L
    int qtd_elementos_diferentes = qtd_caracters(password);

    if (tamanho < 1) {
    return {"Very Weak",0};
    }
    entropia = tamanho * std::log2(qtd_elementos_diferentes); //log e propriedade de expoente aplicada

    //verifica qual é a entropia da senha e retorna
    if (entropia <= 25)
    {
        return {"Very Weak",entropia};
    } else if (entropia <= 45)
    {
        return {"Weak",entropia};
    } else if (entropia <= 80)
    {
        return {"Good",entropia};
    } else if (entropia <= 100)
    {
        return {"Strong",entropia};
    } else if (entropia <= 120)
    {
        return {"Very Strong",entropia};
    } else
    {
        return {"Excellent",entropia};
    }
}

