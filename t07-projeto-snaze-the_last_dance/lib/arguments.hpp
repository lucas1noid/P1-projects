#pragma once
#include <string>
#include <vector>

namespace sg {
    struct GameOptions {
        std::string levelPath;
        int fps = 5;
        int lives = 5;
        int food = 10;
        std::string playerType = "backtracking";
    };

class ArgumentsReader {
public:
    ArgumentsReader();

    void parse(int argc, char* argv[], GameOptions& options);

    void printUsage() const;

};
}
