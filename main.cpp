#include <iostream>
// main.cpp
#include "Hero.hpp"
#include "Enemy.hpp"

// Using tabs instead of spaces
void pressEnterToContinue() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void clearScreen() {
    // Linux/Mac
    system("clear");
}

int getPlayerChoice(int min, int max) {
    int choice;
    while (true) {
        std::cout << "Enter your choice (" << min << "-" << max << "): ";
        std::cin >> choice;

        if (choice >= min && choice <= max) {
            return choice;
        }
        std::cout << "Invalid choice! Try again." << std::endl << std::endl;
    }
}

int main() {
    Hero hero("Aragorn", 12);
    Enemy enemy("Orc", 10);

    std::cout << "========================================" << std::endl;
    std::cout << "       WELCOME TO DICE COMBAT!         " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "YOUR HERO:" << std::endl;
    std::cout << "Name: " << hero.getName() << std::endl;
    std::cout << "Health: " << hero.getHealth() << "/" << hero.getMaxHealth() << std::endl;
    std::cout << std::endl;

    std::cout << "YOUR ENEMY:" << std::endl;
    std::cout << "Name: " << enemy.getName() << std::endl;
    std::cout << "Health: " << enemy.getHealth() << "/" << enemy.getMaxHealth() << std::endl;
    std::cout << std::endl;

    pressEnterToContinue();

		// Game loop starts here
		int round = 1;
		int rerolls = 2;
		while (hero.isAlive() && enemy.isAlive()) {
				clearScreen();

				// Show status at start of round
				std::cout << "========== ROUND " << round << " ==========" << std::endl;
				std::cout << hero.getName() << ": " << hero.getHealth() << "/" << hero.getMaxHealth() << " HP" << std::endl;
				hero.displayDie();

				std::cout << enemy.getName() << ": " << enemy.getHealth() << "/" << enemy.getMaxHealth() << " HP" << std::endl;
				enemy.displayDie();
				std::cout << std::endl;

				// 1. Enemy rolls (behind the scenes)
				DiceFace enemyRoll = enemy.roll();
				std::cout << std::endl;

				// 2. Player's turn - simple and clean
				DiceFace heroRoll = hero.roll();
				std::cout << std::endl;

				while (rerolls > 0) {
						std::cout << "1. Keep this roll" << std::endl;
						std::cout << "2. Reroll (remaining: " << rerolls << ")" << std::endl;
						int choice = getPlayerChoice(1, 2);

						if (choice == 2) {
							std::cout << std::endl << "Rerolling..." << std::endl << std::endl;
							heroRoll = hero.roll();
							rerolls--;
						}
						else {
							std::cout << std::endl << "You chose to keep your roll." << std::endl << std::endl;
							break; // Keep the roll
						}
						std::cout << std::endl;
				}
				rerolls = 2; // Reset rerolls for next round

				// 3. Resolve quickly
				std::cout << "--- RESOLVING TURN ---" << std::endl << std::endl;

				// Hero acts first
				if (heroRoll.action == DAMAGE && heroRoll.target == ENEMY) {
						enemy.takeDamage(heroRoll.value);
				} else if (heroRoll.action == HEAL && heroRoll.target == SELF) {
						hero.heal(heroRoll.value);
				} else if (heroRoll.action == SHIELD && heroRoll.target == SELF) {
						hero.addShield(heroRoll.value);
				} else {
						std::cout << hero.getName() << " does nothing this turn." << std::endl;
				}
				std::cout << std::endl;

				// Enemy acts (if still alive)
				if (enemy.isAlive()) {
						if (enemyRoll.action == DAMAGE && enemyRoll.target == ENEMY) {
								hero.takeDamage(enemyRoll.value);
						} else if (enemyRoll.action == HEAL && enemyRoll.target == SELF) {
								enemy.heal(enemyRoll.value);
						} else if (enemyRoll.action == SHIELD && enemyRoll.target == SELF) {
								enemy.addShield(enemyRoll.value);
						} else {
								std::cout << enemy.getName() << " does nothing this turn." << std::endl;
						}
				}

				hero.resetShield();
				enemy.resetShield();
				std::cout << std::endl;
				pressEnterToContinue();
				round++;
		}

		// Game over
		clearScreen();
		std::cout << "========== GAME OVER ==========" << std::endl;
		if (hero.isAlive()) {
				std::cout << "🎉 " << hero.getName() << " WINS! 🎉" << std::endl;
		} else {
				std::cout << "💀 " << hero.getName() << " DIED! 💀" << std::endl;
		}

    return 0;
}