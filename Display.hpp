// Display.hpp
#ifndef DISPLAY_HPP
# define DISPLAY_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "Die.hpp"
#include "Game.hpp"  // For CombatAction

// Color codes
namespace Colors {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BOLD = "\033[1m";
	}


	class Display {
		public:
			// Status display
			static void showRoundHeader(int round);
			static void showGameOver(bool heroesWin);

			template<typename T>
			static void showHealthBar(const T& character) {
					for (int i = 0; i < character.getMaxHealth(); ++i) {
							if (i < (character.getHealth() - character.getIncomingDamage() + character.getShield())) {
									if constexpr (std::is_same_v<T, Hero>) {
											std::cout << "💚 ";
									} else {
											std::cout << "❤️  ";
									}
							} else if (i < character.getHealth()) {
									std::cout << "💛 ";
							} else {
									std::cout << "🖤 ";
							}
					}
					for (int i = 0; i < (character.getShield() - character.getIncomingDamage()); ++i) {
							std::cout << "🛡️  ";
					}
			}

			template<typename T>
			static void showStatus(const T& character) {
					if constexpr (std::is_same_v<T, Hero>) {
							std::cout << Colors::GREEN << character.getName() << Colors::RESET << ": ";
							showHealthBar(character);
					} else {
							std::cout << Colors::RED << character.getName() << Colors::RESET << ": ";
							showHealthBar(character);
					}
			}

			// Action display
			static void showIntent(const CombatAction& action);
			static void showDiceRoll(const std::string& name, const DiceFace& roll);
			static void showActionResult(const CombatAction& action, const ActionResult& result);

			// Utility
			static void clearScreen();
			static void pressEnterToContinue();
			static void clearLines(int numLines);

			//

};

#endif