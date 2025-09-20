#include <iostream>
#include "Game.hpp"
#include "Display.hpp"
#include "Input.hpp"

int main(void) {
	Display::clearScreen();
	Game game;

	game.playGame();
	return 0;
}