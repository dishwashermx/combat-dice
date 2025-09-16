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
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << std::endl;
		return (0);
}

int Input::getTargetChoice(const std::vector<std::string>& targets) {
    std::cout << "Choose your target:" << std::endl;
    for (size_t i = 0; i < targets.size(); ++i) {
        std::cout << (i + 1) << ". " << targets[i] << std::endl;
    }
    return getPlayerChoice(1, targets.size()) - 1;
}

void Input::pressEnterToContinue() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}
