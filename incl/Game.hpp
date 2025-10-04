#ifndef GAME_HPP
# define GAME_HPP

#include <vector>
#include <random>
#include <iostream>
#include <limits.h>
#include "Die.hpp"
#include "Display.hpp"
#include "Input.hpp"
#include "CharacterFactory.hpp"

class Game {
	friend class Wave;

	private:
		std::vector<Character> heroes;
		std::vector<Character> monsters;
    std::mt19937 generator{std::random_device{}()};  // Initialize generator
		int waveNumber;

		public:
				Game();
				void mainMenu();

				// Character management
				void addMonster();

				// Game state
				bool anyHeroesAlive() const;
				bool anyMonstersAlive() const;
				Character& getRandomAliveHero();

				// Battle
				void playGame();
				void partySetup();
};

#endif