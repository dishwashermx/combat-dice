#include "Display.hpp"
#include "Wave.hpp"

void Display::showHeader(std::string type, int number) {
    std::string prefix = "========== ";
    std::string headerString = type + " " + std::to_string(number);
    std::string suffix = " ==========";

    // Clear some space for the animation
    std::cout << std::endl;

    // Animate string word
    for (size_t charPos = 0; charPos < headerString.length(); ++charPos) {
        // Show rolling characters for current position
        for (int rollFrame = 0; rollFrame < 4; ++rollFrame) {
            // Move cursor back to start
            std::cout << "\r" << Colors::BOLD << Colors::CYAN << prefix << Colors::RESET;

            // Print already settled letters
            for (size_t i = 0; i < charPos; ++i) {
                std::cout << Colors::BOLD << Colors::YELLOW << headerString[i] << Colors::RESET;
            }

            // Show rolling character for current position
            char rollingChar = 'A' + (rand() % 26);
            std::cout << Colors::YELLOW << rollingChar << Colors::RESET;

            // Show rolling characters for ALL remaining positions
            for (size_t i = charPos + 1; i < headerString.length(); ++i) {
                char futureRollingChar;
                if (headerString[i] == ' ') {
                    futureRollingChar = ' '; // Keep spaces as spaces
                } else if (isdigit(headerString[i])) {
                    futureRollingChar = '0' + (rand() % 10); // Random digit
                } else {
                    futureRollingChar = 'A' + (rand() % 26); // Random letter
                }
                std::cout << Colors::YELLOW << futureRollingChar << Colors::RESET;
            }

            std::cout << Colors::BOLD << Colors::CYAN << suffix << Colors::RESET;
            std::cout << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

		// Finally, print the complete settled line
		std::cout << "\r" << Colors::BOLD << Colors::CYAN << prefix << Colors::RESET
		          << Colors::BOLD << Colors::YELLOW << headerString << Colors::RESET
		          << Colors::BOLD << Colors::CYAN << suffix << Colors::RESET;
		std::cout << std::flush << std::endl;

    // Brief pause before continuing
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void Display::showIntent(const CombatAction& action) {

	if (action.roll.action == EMPTY) {
		return;
	}

		typeText(Colors::RED + action.actorName + Colors::RESET + " intends to ", 10);
		// Build the action string with colors
		// Use description if available, otherwise fallback to actionToString
		std::string actionText = action.roll.desc.empty() ?
				Colors::BOLD + actionToString(action.roll.action) + Colors::RESET :
				"use " + Colors::BOLD + action.roll.name + Colors::RESET + " on";

		// Add target with color based on team
    if (!action.targetName.empty() && action.targetTeam == 1)
        actionText += " " + Colors::GREEN + action.targetName + Colors::RESET;
    else
				actionText += " " + Colors::RED + action.targetName + Colors::RESET;

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
	std::string actorName = action.actorName;
	std::string targetName = action.targetName;
	std::string actionValue = std::to_string(action.roll.value);

	if (action.roll.action == EMPTY) {
		return ;
	}

	if (action.roll.action != DODGE && action.roll.action != STUN) {
		std::string output = Colors::GREEN + actorName + Colors::RESET + " ";

		// Use description if available, otherwise fallback to actionToString + "s"
		if (!action.roll.desc.empty()) {
			output += Colors::BOLD + action.roll.desc + Colors::RESET + " ";
		} else {
			output += Colors::BOLD + actionToString(action.roll.action) + "s " + Colors::RESET;
		}

		if (!targetName.empty() && targetName != actorName) {
			output += Colors::RED + targetName + Colors::RESET + " ";
		}

		// Show value for attack, heal, block actions
		output += "for " + Colors::BOLD + actionValue + Colors::RESET;
		std::cout << output;
	}

	if (action.roll.action == ATTACK) {
		std::string attackOutput = " damage!\n";
		if (result.wasDodged) {
			attackOutput += Colors::YELLOW + targetName + " dodged the attack!" + Colors::RESET + "\n";
		} else {
			if (result.damageBlocked > 0) {
				attackOutput += Colors::BLUE + targetName + " blocked " + std::to_string(result.damageBlocked) + " damage!" + Colors::RESET + "\n";
			}
			if (result.damageDealt > 0) {
				attackOutput += Colors::RED + targetName + " took " + std::to_string(result.damageDealt) + " damage!" + Colors::RESET + "\n";
				attackOutput += targetName + " HP left: " + Colors::BOLD + std::to_string(result.newHealth) + Colors::RESET + "\n";
			}
		}
		std::cout << attackOutput;
	} else if (action.roll.action == HEAL) {
		std::string healOutput = " HP!\n";
		if (result.healingApplied > 0) {
			healOutput += Colors::GREEN + targetName + " healed for " + std::to_string(result.healingApplied) + " HP!" + Colors::RESET + "\n";
		}
		std::cout << healOutput;
	} else if (action.roll.action == BLOCK) {
		std::cout << " shield!\n";
	} else if (action.roll.action == DODGE) {
		std::cout << "\n" << Colors::YELLOW << actorName << " is ready to dodge!" << Colors::RESET << "\n";
	} else if (action.roll.action == STUN) {
		// Handle complete stun message
		std::string stunOutput = Colors::GREEN + actorName + Colors::RESET + " " + Colors::BOLD + "stuns " + Colors::RESET;
		if (!targetName.empty() && targetName != actorName) {
			stunOutput += Colors::RED + targetName + Colors::RESET;
		}
		stunOutput += "!\n";
		std::cout << stunOutput;

		if (result.wasDodged) {
			std::cout << Colors::YELLOW << targetName << " dodged the stun!" << Colors::RESET << std::endl;
		} else if (result.wasStunned) {
			std::cout << Colors::YELLOW << targetName << " is stunned!" << Colors::RESET << std::endl;
		}
		// No message when stun fails due to HP being too high - it simply doesn't connect
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
		std::cout << Colors::RED << Colors::BOLD << "💀 MONSTERS WIN! 💀" << Colors::RESET << std::endl;
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
						" ",                             // Name placeholder
						" ",                             // Description placeholder
						" ",                             // Emoji placeholder
            static_cast<Action>(rand() % 6),  // Random action (0-5 for all 6 actions)
            1 + (rand() % 6),                 // Random value 1-6
            NO_MARK                           // No marks for animation
        );

        // Get proper emoji for animation
        std::string emoji;
        if (tempRoll.action == ATTACK) {
            emoji = "⚔️";
        } else if (tempRoll.action == HEAL) {
            emoji = "❤️";
        } else if (tempRoll.action == BLOCK) {
            emoji = "🛡️";
        } else if (tempRoll.action == DODGE) {
            emoji = "💨";
        } else if (tempRoll.action == STUN) {
            emoji = "⚡";
        } else {
            emoji = "❌";
        }

        std::cout << "\r🎲  :: " << Colors::BOLD << tempRoll.value << " "
									<< Colors::RESET << emoji << "  (" << Colors::YELLOW
                  << actionToString(tempRoll.action) << Colors::RESET << ")"
                  << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(90));
    }

    // Final actual roll (this is the one that counts)
    DiceFace finalRoll = character.roll();

    // Get proper emoji for final roll
    std::string finalEmoji;
    if (finalRoll.action == ATTACK) {
        finalEmoji = "⚔️";
    } else if (finalRoll.action == HEAL) {
        finalEmoji = "❤️";
    } else if (finalRoll.action == BLOCK) {
        finalEmoji = "🛡️";
    } else if (finalRoll.action == DODGE) {
        finalEmoji = "💨";
    } else if (finalRoll.action == STUN) {
        finalEmoji = "⚡";
    } else if (finalRoll.action == EMPTY) {
        finalEmoji = "❌";
    } else {
        finalEmoji = "✨";
    }
    // Use the name if available, otherwise fallback to actionToString
    std::string rollName = finalRoll.name.empty() ? actionToString(finalRoll.action) : finalRoll.name;

    std::cout << "\r🎲  :: " << Colors::BOLD << finalRoll.value << " "
              << Colors::RESET << finalEmoji << "  (" << Colors::YELLOW
              << rollName << Colors::RESET << ")!"
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
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << Colors::BOLD << Colors::YELLOW << "Welcome to the Ultimate Dice Battle Arena!" << Colors::RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(700));
    std::cout << "Roll your way to victory in epic combat encounters!" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(700));
    std::cout << "May fortune favor the bold!" << std::endl << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(300));

    Display::typeText(Colors::BOLD + Colors::RED + "Press Enter to start your adventure..." + Colors::RESET, 40);
}

void Display::displayFaces(const std::vector<DiceFace>& faces) {
		for (size_t i = 0; i < faces.size(); i++) {
				const DiceFace& face = faces[i];

				// Use the emoji from DiceFace if available, otherwise fallback to action-based emojis
				std::string emoji = face.emoji;
				if (emoji.empty()) {
						if (face.action == ATTACK) {
								emoji = "⚔️";
						} else if (face.action == HEAL) {
								emoji = "💚";
						} else if (face.action == BLOCK) {
								emoji = "🛡️";
						} else {
								emoji = "✖";
						}
				}

				// Choose color based on action
				std::string color;
				if (face.action == ATTACK) {
						color = Colors::RED;
				} else if (face.action == HEAL) {
						color = Colors::GREEN;
				} else if (face.action == BLOCK) {
						color = Colors::BLUE;
				} else {
						color = Colors::GREY;
				}

				// Display the dice face with emoji and value
				std::cout << color << "[";

				std::cout << emoji << "  ";

								if (face.marks != NO_MARK) {
										std::cout << marksToEmoji(face.marks);
										std::cout << " ";
								}

				std::cout << face.value << "]" << Colors::RESET;

				if (i < faces.size() - 1) {
						std::cout << " ";
				}
		}
}

std::string Display::marksToEmoji(int marks) {
		std::string markEmojis = "";

		if (marks & SPLASH) {
				markEmojis += "💦";  // SPLASH - affects adjacent targets
		}
		if (marks & WAVE) {
				markEmojis += "🌊";  // WAVE - affects target team
		}
		if (marks & QUAKE) {
				markEmojis += "⛰️";  // QUAKE - affects everyone alive
		}

		return markEmojis;
}

void Display::showWaveHeader(int wave) {
    showHeader("Wave", wave);
}

void Display::showRoundHeader(int round) {
    showHeader("Round", round);
}