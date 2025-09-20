#include "Display.hpp"
#include "Wave.hpp"

void Display::showRoundHeader(int round) {
    std::string prefix = "========== ";
    std::string roundString = "ROUND " + std::to_string(round);
    std::string suffix = " ==========";

    // Clear some space for the animation
    std::cout << std::endl;

    // Animate "ROUND" word
    for (size_t charPos = 0; charPos < roundString.length(); ++charPos) {
        // Show rolling characters for current position
        for (int rollFrame = 0; rollFrame < 3; ++rollFrame) {
            // Move cursor back to start
            std::cout << "\r" << Colors::BOLD << Colors::CYAN << prefix << Colors::RESET;

            // Print already settled letters
            for (size_t i = 0; i < charPos; ++i) {
                std::cout << Colors::BOLD << Colors::YELLOW << roundString[i] << Colors::RESET;
            }

            // Show rolling character for current position
            char rollingChar = 'A' + (rand() % 26);
            std::cout << Colors::YELLOW << rollingChar << Colors::RESET;

            // Show rolling characters for ALL remaining positions
            for (size_t i = charPos + 1; i < roundString.length(); ++i) {
                char futureRollingChar;
                if (roundString[i] == ' ') {
                    futureRollingChar = ' '; // Keep spaces as spaces
                } else if (isdigit(roundString[i])) {
                    futureRollingChar = '0' + (rand() % 10); // Random digit
                } else {
                    futureRollingChar = 'A' + (rand() % 26); // Random letter
                }
                std::cout << Colors::YELLOW << futureRollingChar << Colors::RESET;
            }

            std::cout << Colors::BOLD << Colors::CYAN << suffix << Colors::RESET;
            std::cout << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

		// Finally, print the complete settled line
		std::cout << "\r" << Colors::BOLD << Colors::CYAN << prefix << Colors::RESET;
		std::cout << Colors::BOLD << Colors::YELLOW << roundString << Colors::RESET;
		std::cout << Colors::BOLD << Colors::CYAN << suffix << Colors::RESET;
		std::cout << std::flush << std::endl;

    // Brief pause before continuing
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void Display::showIntent(const CombatAction& action) {
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
	<< Colors::BOLD << actionToString(action.roll.action) << "s " << Colors::RESET;

	if (!action.targetName.empty() && action.targetName != action.actorName) {
		std::cout << Colors::RED << action.targetName << Colors::RESET;
		std::cout << " ";
	}

	std::cout << "for " << Colors::BOLD << action.roll.value << Colors::RESET;

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

    // Show spinning animation with fake rolls (don't affect character state)
    for (int i = 0; i < 10; ++i) {
        // Create temporary fake roll for animation with all required parameters
        DiceFace tempRoll(
            static_cast<Action>(rand() % 4),  // Random action
            static_cast<Target>(rand() % 3),  // Random target
            1 + (rand() % 6)                  // Random value 1-6
        );

        std::cout << "\r🎲 " << tempRoll.value << " ("
                  << actionToString(tempRoll.action) << ")" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(90));
    }

    // Final actual roll (this is the one that counts)
    DiceFace finalRoll = character.roll();
    std::cout << "\r🎲  " << Colors::BOLD << finalRoll.value
              << Colors::RESET << " (" << Colors::YELLOW
              << actionToString(finalRoll.action) << Colors::RESET << ")!"
              << std::endl;

    return finalRoll;
}

void Display::titleScreen() {
    std::cout << Colors::BOLD << Colors::CYAN;
    std::cout << " ____  _                  ______ _       _     _       " << std::endl;
    std::cout << "|  _ \\(_)                |  ____(_)     | |   | |      " << std::endl;
    std::cout << "| | | |_  ___ ___ _   _  | |__   _  __ _| |__ | |_ ___ " << std::endl;
    std::cout << "| | | | |/ __/ _ \\ | | | |  __| | |/ _` | '_ \\| __/ __|" << std::endl;
    std::cout << "| |_| | | (_|  __/ |_| | | |    | | (_| | | | | |_\\__ \\" << std::endl;
    std::cout << "|____/|_|\\___\\___|\\__, | |_|    |_|\\__, |_| |_|\\__|___/" << std::endl;
    std::cout << "                   __/ |            __/ |             " << std::endl;
    std::cout << "                  |___/            |___/              " << Colors::RESET << std::endl;
    std::cout << std::endl;
    std::cout << Colors::BOLD << Colors::YELLOW << "Welcome to the Ultimate Dice Battle Arena!" << Colors::RESET << std::endl;
    std::cout << "Roll your way to victory in epic combat encounters!" << std::endl;
    std::cout << "May fortune favor the bold!" << std::endl << std::endl;

    Display::blinkText(Colors::BOLD + Colors::RED + "Press Enter to start your adventure..." + Colors::RESET, 3);
}

void Display::showWaveHeader(int waveNumber) {
		std::string prefix = "========== ";
		std::string waveString = "WAVE " + std::to_string(waveNumber);
		std::string suffix = " ==========";

		// Clear some space for the animation
		std::cout << std::endl;

		// Animate "WAVE" word
		for (size_t charPos = 0; charPos < waveString.length(); ++charPos) {
				// Show rolling characters for current position
				for (int rollFrame = 0; rollFrame < 3; ++rollFrame) {
						// Move cursor back to start
						std::cout << "\r" << Colors::BOLD << Colors::CYAN << prefix << Colors::RESET;

						// Print already settled letters
						for (size_t i = 0; i < charPos; ++i) {
								std::cout << Colors::BOLD << Colors::YELLOW << waveString[i] << Colors::RESET;
						}

						// Show rolling character for current position
						char rollingChar = 'A' + (rand() % 26);
						std::cout << Colors::YELLOW << rollingChar << Colors::RESET;

						// Show rolling characters for ALL remaining positions
						for (size_t i = charPos + 1; i < waveString.length(); ++i) {
								char futureRollingChar;
								if (waveString[i] == ' ') {
										futureRollingChar = ' '; // Keep spaces as spaces
								} else if (isdigit(waveString[i])) {
										futureRollingChar = '0' + (rand() % 10); // Random digit
								} else {
										futureRollingChar = 'A' + (rand() % 26); // Random letter
								}
								std::cout << Colors::YELLOW << futureRollingChar << Colors::RESET;
						}

						std::cout << Colors::BOLD << Colors::CYAN << suffix << Colors::RESET;
						std::cout << std::flush;
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
		}

		// Finally, print the complete settled line
		std::cout << "\r" << Colors::BOLD << Colors::CYAN << prefix << Colors::RESET;
		std::cout << Colors::BOLD << Colors::YELLOW << waveString << Colors::RESET;
		std::cout << Colors::BOLD << Colors::CYAN << suffix << Colors::RESET;
		std::cout << std::flush << std::endl;
		// Brief pause before continuing
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
}