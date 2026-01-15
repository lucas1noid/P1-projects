#ifndef FUNCTION__HPP
#define FUNCTION__HPP


//THIS LIBS WILL BE REPLACE TO THE .CPP THAT THEY'RE USED!!!!
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>

#include "groups.hpp"

// LIB. ADD
#include <cstring>
#include <random> 
#include <array>
#include <cmath>
  
void usage (std::string_view msg);

void help_doit();  

void validate_arguments(int argc, char* argv[], RunningOptions& run_options);
  
int die (int first, int last);
  
std::string generate_password (const RunningOptions& run_options);
  
int qtd_groups(std::string_view password);

std::pair <std::string,int> password_quality(std::string_view password);



#endif


