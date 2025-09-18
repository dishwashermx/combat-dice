#include <iostream>
#include "Game.hpp"
#include "Display.hpp"
#include "Input.hpp"

int main(void) {
	Display::clearScreen();

	std::cout << "========================================" << std::endl;
	std::cout << "       WELCOME TO DICE COMBAT!         " << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << std::endl;

	Game game;

	// Setup heroes
	game.addHero();

	// Setup enemies
	game.addMonster();

	// Start the game
	game.playGame();
	return 0;
}