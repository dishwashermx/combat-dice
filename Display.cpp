#include "Display.hpp"

void Display::showRoundHeader(int round) {
	std::cout << Colors::BOLD << Colors::CYAN
						<< "========== ROUND " << round << " =========="
						<< Colors::RESET << std::endl;
}

void Display::showIntent(const CombatAction& action) {
    std::cout << Colors::RED << action.actorName << Colors::RESET << " intends to ";

    if (action.roll.action == EMPTY) {
        std::cout << Colors::MAGENTA << "do nothing..." << Colors::RESET << std::endl;
        return;
    }

    std::cout << actionToString(action.roll.action);
    if (!action.targetName.empty()) {
        std::cout << " " << Colors::GREEN << action.targetName << Colors::RESET;
    }

    std::cout << " for " << Colors::BOLD << action.roll.value << Colors::RESET;

    // Add appropriate suffix
    if (action.roll.action == ATTACK) {
        std::cout << " damage!";
    } else if (action.roll.action == HEAL) {
        std::cout << " HP!";
    } else if (action.roll.action == BLOCK) {
        std::cout << " shield!";
    }

    std::cout << std::endl;
}

void Display::showDiceRoll(const std::string& name, const DiceFace& roll) {
    std::cout << Colors::GREEN << name << Colors::RESET << " rolls a "
              << Colors::BOLD << roll.value << Colors::RESET << " ("
              << Colors::YELLOW << actionToString(roll.action) << Colors::RESET << ")!" << std::endl;
}

void Display::showActionResult(const CombatAction& action, const ActionResult& result) {
		if (action.roll.action == EMPTY) {
				std::cout << Colors::MAGENTA << action.actorName << " does nothing..." << Colors::RESET << std::endl;
				return ;
		}
		std::cout << Colors::GREEN << action.actorName << Colors::RESET << " uses "
							<< Colors::BOLD << actionToString(action.roll.action) << Colors::RESET;

		if (!action.targetName.empty() && action.targetName != action.actorName) {
				std::cout << " on " << Colors::RED << action.targetName << Colors::RESET;
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
				if (result.newShield > 0) {
						std::cout << Colors::GREEN << "Shield left: " << result.newShield << Colors::RESET << std::endl;
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