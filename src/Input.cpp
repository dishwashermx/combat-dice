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

int Input::getTargetChoice(const std::vector<Character>& characters) {
		std::cout << "Choose your target:" << std::endl;

		for (size_t i = 0; i < characters.size(); ++i) {
				std::cout << (i + 1) << ". " << characters[i].getName() << ": ";
				Display::showHealthBar(characters[i]);
				std::cout << std::endl;
		}

		return getPlayerChoice(1, characters.size()) - 1;
}

void Input::pressEnterToContinue(std::string message) {
    std::cout << message;
    std::cin.ignore();
    std::cin.get();
		Display::clearLines(2);
}
