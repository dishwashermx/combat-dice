#ifndef INPUT_HPP
# define INPUT_HPP
#include <iostream>
#include <limits>
#include <vector>
#include <string>

class Input {
public:
    static int getPlayerChoice(int min, int max);
    static int getTargetChoice(const std::vector<std::string>& targets);
    static void pressEnterToContinue();
};

#endif