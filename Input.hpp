#ifndef INPUT_HPP
# define INPUT_HPP
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include "Enemy.hpp"
#include "Hero.hpp"
#include "Display.hpp"

class Input {
public:
    static int getPlayerChoice(int min, int max);
    static void pressEnterToContinue();

    // Template method declaration and definition
    template<typename T>
    static int getTargetChoice(const std::vector<T>& characters) {
        std::cout << "Choose your target:" << std::endl;

        for (size_t i = 0; i < characters.size(); ++i) {
            std::cout << (i + 1) << ". " << characters[i].getName() << ": ";
            Display::showHealthBar(characters[i]);
            std::cout << std::endl;
        }

        return getPlayerChoice(1, characters.size()) - 1;
    }
};

#endif