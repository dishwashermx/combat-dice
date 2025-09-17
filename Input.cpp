#include "Input.hpp"
#include <cstdlib>

int Input::getPlayerChoice(int min, int max) {
    int choice;
    while (true) {
        std::cout << "Enter your choice (" << min << "-" << max << "): ";
        std::cin >> choice;

        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice! Try again." << std::endl << std::endl;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << std::endl;
            return choice;
        }
    }
		return -1; // Should never reach here
}

// Add explicit template instantiations at the end of the file:
template int Input::getTargetChoice<Hero>(const std::vector<Hero>& characters);
template int Input::getTargetChoice<Enemy>(const std::vector<Enemy>& characters);

void Input::pressEnterToContinue() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}
