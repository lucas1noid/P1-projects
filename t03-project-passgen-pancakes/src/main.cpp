#include <cstdlib>
#include <iostream>
#include <string>
#include "groups.hpp"
#include "functions.hpp"


int main(int argc, char* argv[]) {
  RunningOptions run_options;  // Stores incoming arguments.
  // (1) Validate command line arguments.
  std::cout << "Welcome to passgen v 1.0, © DIMAp 2025\n\n";
  validate_arguments(argc, argv, run_options); 
  
  if (run_options.error) {
    return EXIT_FAILURE;
  }

  // (2) Generate password.
  // GENERETE THE PASSWORD IF EVERYTHING IS OK
  auto password = generate_password(run_options);
  std::cout << ">>> The generated password is:\n" << password << "\n\n";
  // (3) Show password quality if requested.
  if (run_options.show_password_strength) {
    std::cout << ">>> Password strength: " << password_quality(password).first << "\n\n";
    std::cout << ">>> Password bits: " << password_quality(password).second << "\n\n";
  }

  return EXIT_SUCCESS;
}
