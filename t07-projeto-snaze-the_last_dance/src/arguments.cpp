#include "../lib/arguments.hpp"
#include <iostream>
#include <string>

namespace sg {

ArgumentsReader::ArgumentsReader(){};
    
void ArgumentsReader::parse(int argc, char* argv[], GameOptions& options){
    if(argc < 2){
        printUsage();
        exit(0);
    }
    for(int i = 1; i < argc; ++i){
        std::string arg = argv[i];
        if(arg == "--help"){
            printUsage();
            exit(0);
        } else if(arg == "--fps" && i + 1 < argc){
            try
            {
                options.fps = std::stoi(argv[++i]);
            }
            catch(const std::exception& e)
            {
                std::cerr << "Invalid Argument..." << '\n';
                printUsage();
                exit(1);
            }
            
        } else if(arg == "--lives" && i + 1 < argc){
            try
            {
                options.lives = std::stoi(argv[++i]);
            }
            catch(const std::exception& e)
            {
                std::cerr << "Invalid Argument..." << '\n';
                printUsage();
                exit(1);
            }
            
        } else if(arg == "--food" && i + 1 < argc){
            try
            {
                options.food = std::stoi(argv[++i]);
            }
            catch(const std::exception& e)
            {
                std::cerr << "Invalid Argument..." << '\n';
                printUsage();
                exit(1);
            }
            
        } else {
            try
            {
                options.levelPath = arg;
            }
            catch(const std::exception& e)
            {
                std::cerr << "Invalid Argument..." << '\n';
                printUsage();
                exit(1);
            }
            
        }
        // else if(arg == "--playertype" && i + 1 < argc){
        //     options.playerType = argv[++i];
        // } 
    }
}

void ArgumentsReader::printUsage() const {
        std::cout << "Usage: snaze [<options>] <input_level_file>\n"
                  << "  Game simulation options:\n"
                  << "    --help               Print this help text.\n"
                  << "    --fps <num>          Number of frames (board) presented per second.\n"
                  << "    --lives <num>        Number of lives the snake shall have. Default = 5.\n"
                  << "    --food <num>         Number of food pellets for the entire simulation. Default = 10.\n";
                  //<< "    --playertype <type>  Type of snake intelligence: random, backtracking. Default = backtracking\n";
    }
}