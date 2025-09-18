#ifndef INPUT_HPP
# define INPUT_HPP
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include "Display.hpp"

class Input {
public:
    static int getPlayerChoice(int min, int max);
    static void pressEnterToContinue();

    static int getTargetChoice(const std::vector<Character>& characters);
};

#endif