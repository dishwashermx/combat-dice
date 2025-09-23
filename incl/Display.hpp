// Display.hpp
#ifndef DISPLAY_HPP
# define DISPLAY_HPP

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <thread>
#include <chrono>

#include "Die.hpp"
#include "Character.hpp"


struct CombatAction;
struct ActionResult;

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
		const std::string GREY = "\033[90m";
	}


	class Display {
		public:
			// Status display
			static void titleScreen();
			static void showHeader(std::string type, int number);
			static void showWaveHeader(int wave);
			static void showRoundHeader(int round);
			static void showGameOver(bool heroesWin);

			template<typename T>
			static void showHealthBar(const T& character) {
				int health = character.getHealth();
				int maxHealth = character.getMaxHealth();
				int shield = character.getShield();
				int incomingDamage = character.getIncomingDamage();

				// Calculate damage distribution
				int damageToShield = std::min(shield, incomingDamage);
				int damageToHealth = std::max(0, incomingDamage - shield);

				// Show health hearts (current health minus damage that gets through shield)
				for (int i = 0; i < maxHealth; ++i) {
					if (i < (health - damageToHealth)) {
						if (character.getTeam() == "Hero") {
							std::cout << "💚 ";
						} else {
							std::cout << "❤️  ";
						}
					} else if (i < health) {
						// Health that will be lost (yellow hearts)
						std::cout << "💛 ";
					} else {
						std::cout << "🖤 ";
					}
				}

				// Show remaining shield after blocking damage
				int remainingShield = std::max(0, shield - damageToShield);
				for (int i = 0; i < remainingShield; ++i) {
					std::cout << "🛡️  ";
				}
			}

			template<typename T>
			static void showStatus(const T& character) {
					if (character.getTeam() == "Hero") {
							std::cout << Colors::GREEN << character.getName() << Colors::RESET << ": ";
					} else {
							std::cout << Colors::RED << character.getName() << Colors::RESET << ": ";
						}
					showHealthBar(character);
					}

			// Action display
			static void displayFaces(const std::vector<DiceFace>& faces);
			static void showIntent(const CombatAction& action);
			static DiceFace animatedRoll(Character& character);
			static void showActionResult(const CombatAction& action, const ActionResult& result);

			// Utility
			static void clearScreen();
			static void pressEnterToContinue();
			static void clearLines(int numLines);
			static std::string marksToEmoji(int marks);

			//Aesthetic/UX Enhancements
			static void typeText(const std::string& text, int delayMs = 50);
			static void blinkText(const std::string& text, int blinks = 3);
};

#endif