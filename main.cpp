#include <iostream>
#include "Hero.hpp"
#include "Enemy.hpp"
#include "Game.hpp"
#include "Display.hpp"
#include "Input.hpp"

// int main() {
// 		clearScreen();
// 		std::cout << "Welcome to the Dice Combat Game!" << std::endl << std::endl;

// 		Game game;

// 		// Setup heroes
// 		int numHeroes;
// 		std::cout << "Enter number of heroes (1-4): ";
// 		numHeroes = getPlayerChoice(1, 4);
// 		for (int i = 0; i < numHeroes; ++i) {
// 				std::string name;
// 				int health;
// 				std::cout << "Enter name for Hero " << (i + 1) << ": ";
// 				std::cin >> name;
// 				std::cout << "Enter health for " << name << " (10-30): ";
// 				health = getPlayerChoice(10, 30);
// 				game.addHero(name, health);
// 				clearScreen();
// 		}

// 		// Setup enemies
// 		int numEnemies;
// 		std::cout << "Enter number of enemies (1-4): ";
// 		numEnemies = getPlayerChoice(1, 4);
// 		for (int i = 0; i < numEnemies; ++i) {
// 				std::string name;
// 				int health;
// 				std::cout << "Enter name for Enemy " << (i + 1) << ": ";
// 				std::cin >> name;
// 				std::cout << "Enter health for " << name << " (10-30): ";
// 				health = getPlayerChoice(10, 30);
// 				game.addEnemy(name, health);
// 				clearScreen();
// 		}

// 		// Start the game
// 		game.playGame();

// 		return 0;
// }

int main(void) {
	Display::clearScreen();

	std::cout << "========================================" << std::endl;
	std::cout << "       WELCOME TO DICE COMBAT!         " << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << std::endl;

	Game game;

	// Setup heroes
	game.addHero("Aragorn", 10);
	game.addHero("Legolas", 15);

	// Setup enemies
	game.addEnemy("Orc", 7);
	game.addEnemy("Goblin", 5);
	game.addEnemy("Troll", 12);

	// Start the game
	game.playGame();
	return 0;
}