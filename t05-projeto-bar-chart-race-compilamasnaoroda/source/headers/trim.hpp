#pragma once

#include <string>
#include <algorithm>

/*!
* Removes the initial characters from any string.
* 
* @param s The string to be changed.
* @param t A C string containing the set of characters to be removed.
* 
* @return The new string with the initial characters removed.
*/
inline std::string ltrim(const std::string& s, const char* t) {
  std::string clone{ s };
  clone.erase(0, clone.find_first_not_of(t));
  return clone;
}

/*!
* Removes trailing unimportant characters from any string.
* 
* @param s The string to be changed.
* @param t A C string containing the set of characters to be removed.
* 
* @return the new string with trailing characters removed.
*/
inline std::string rtrim(const std::string& s, const char* t) {
    std::string clone{ s };
    clone.erase(clone.find_last_not_of(t) + 1);
    return clone;
}


/*!
*
* Removes both the leading and trailing characters from a string.
*
* @param s The string to be changed.
* @param t A C-style string containing the set of characters to be removed.
*
* @return The new string with the leading and trailing characters removed.
*/
inline std::string trim(const std::string& s, const char* t = " ") {
    return rtrim(ltrim(s, t), t);
}