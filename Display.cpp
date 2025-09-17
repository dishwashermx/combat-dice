#include "Display.hpp"
#include "Game.hpp"  // Now include Game.hpp here for the full definitions
#include "Hero.hpp"
#include "Enemy.hpp"

void Display::showRoundHeader(int round) {
	std::cout << Colors::BOLD << Colors::CYAN
						<< "========== ROUND " << round << " =========="
						<< Colors::RESET << "\a" << std::endl;
}

void Display::showIntent(const CombatAction& action) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    typeText(Colors::RED + action.actorName + Colors::RESET + " intends to ", 10);

    if (action.roll.action == EMPTY) {
        typeText(Colors::MAGENTA + "do nothing..." + Colors::RESET, 20);
        return;
    }

    // Build the action string with colors
    std::string actionText = Colors::BOLD + actionToString(action.roll.action) + Colors::RESET;
    if (!action.targetName.empty()) {
        actionText += " " + Colors::GREEN + action.targetName + Colors::RESET;
    }
    actionText += " for " + Colors::YELLOW + std::to_string(action.roll.value) + Colors::RESET;

    // Add suffix based on action type
    if (action.roll.action == ATTACK) {
        actionText += Colors::YELLOW + " damage!" + Colors::RESET;
    } else if (action.roll.action == HEAL) {
        actionText += Colors::YELLOW + " HP!" + Colors::RESET;
    } else if (action.roll.action == BLOCK) {
        actionText += Colors::YELLOW + " shield!" + Colors::RESET;
    }

    typeText(actionText, 20);
}

void Display::showActionResult(const CombatAction& action, const ActionResult& result) {
	if (action.roll.action == EMPTY) {
		std::cout << Colors::MAGENTA << action.actorName << " does nothing..." << Colors::RESET << std::endl;
		return ;
	}
	std::cout << Colors::GREEN << action.actorName << Colors::RESET << " "
	<< Colors::BOLD << actionToString(action.roll.action) << "s" << Colors::RESET;

	if (!action.targetName.empty() && action.targetName != action.actorName) {
		std::cout << Colors::RED << action.targetName << Colors::RESET;
	}

	std::cout << " for " << Colors::BOLD << action.roll.value << Colors::RESET;

	if (action.roll.action == ATTACK) {
		std::cout << " damage!" << std::endl;
	if (result.damageBlocked > 0) {
		std::cout << Colors::BLUE << action.targetName << "'s shield absorbed " << result.damageBlocked << " damage!" << Colors::RESET << std::endl;
	}
	if (result.damageDealt > 0) {
		std::cout << Colors::RED << action.targetName << " took " << result.damageDealt << " damage!" << Colors::RESET << std::endl;
		std::cout << action.targetName << " HP left: " << Colors::BOLD << result.newHealth << Colors::RESET << std::endl;
	}
	} else if (action.roll.action == HEAL) {
	std::cout << " HP!" << std::endl;
	if (result.healingApplied > 0) {
		std::cout << Colors::GREEN << action.targetName << " healed for " << result.healingApplied << " HP!" << Colors::RESET << std::endl;
	}
	} else if (action.roll.action == BLOCK) {
			std::cout << " shield!" << std::endl;
	}
}

		void Display::clearScreen() {
			#ifdef _WIN32
			system("cls");
			#else
			system("clear");
			#endif
		}

		void Display::showGameOver(bool heroesWin) {
			if (heroesWin) {
				std::cout << Colors::GREEN << Colors::BOLD << "🎉 HEROES WIN! 🎉" << Colors::RESET << std::endl;
			} else {
				std::cout << Colors::RED << Colors::BOLD << "💀 ENEMIES WIN! 💀" << Colors::RESET << std::endl;
			}
		}

		void Display::clearLines(int numLines) {
			for (int i = 0; i < numLines; ++i) {
				std::cout << "\033[A";  // Move cursor up one line
        std::cout << "\033[K";  // Clear line
			}
		}

		void Display::typeText(const std::string& text, int delayMs) {
			bool inColorCode = false;

			for (size_t i = 0; i < text.length(); ++i) {
				char c = text[i];

        // Detect color escape sequences
        if (c == '\033') {
					inColorCode = true;
        }

        std::cout << c << std::flush;

        // Only delay on visible characters (not color codes)
        if (!inColorCode) {
					std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        // End of color code
        if (inColorCode && c == 'm') {
					inColorCode = false;
        }
    }
}

void Display::blinkText(const std::string& text, int blinks) {
	for (int i = 0; i < blinks; ++i) {
		std::cout << "\r" << Colors::BOLD << text << Colors::RESET << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		std::cout << "\r" << std::string(text.length(), ' ') << "\r" << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
	std::cout << Colors::BOLD << text << Colors::RESET << std::endl;
}

DiceFace Display::animatedRoll(Character& character) {
	std::cout << Colors::GREEN << character.getName() << Colors::RESET << " rolls a "
						<< Colors::BOLD << "..." << Colors::RESET << std::endl;

	// Show spinning animation
	for (int i = 0; i < 10; ++i) {
		DiceFace tempRoll = character.roll();
		std::cout << "\r🎲 " << tempRoll.value << " ("
		<< actionToString(tempRoll.action) << ")" << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(90));
	}

	// Final roll
	DiceFace finalRoll = character.roll();
	std::cout << "\r🎲  " << Colors::BOLD << finalRoll.value
	<< Colors::RESET << " (" << Colors::YELLOW
	<< actionToString(finalRoll.action) << Colors::RESET << ")!"
	<< std::endl;

	return finalRoll;
}