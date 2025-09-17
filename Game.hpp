#ifndef GAME_HPP
# define GAME_HPP

#include <vector>
#include <random>
#include <iostream>
#include <limits.h>
#include "Die.hpp"
#include "Hero.hpp"
#include "Enemy.hpp"

struct CombatAction
{
	DiceFace roll;
	int actorIndex;
	int targetTeam; // 0 for hero, 1 for enemy
	int targetIndex;
	std::string actorName;
	std::string targetName;

	CombatAction(DiceFace r, int aIdx, int tTeam, int tIdx, const std::string& aName, const std::string& tName)
	: roll(r), actorIndex(aIdx), targetTeam(tTeam), targetIndex(tIdx), actorName(aName), targetName(tName) {}
};


class Game {
	private:
		std::vector<Hero> heroes;
		std::vector<Enemy> enemies;
    std::mt19937 generator{std::random_device{}()};  // Initialize generator
		int round;

	public:
		Game();

		// Setup methods
		void addHero(const std::string& name, int health);
		void addEnemy(const std::string& name, int health);

		// Victory conditions
		bool isGameOver() const;

		// Helper methods
		bool anyHeroesAlive() const;
		bool anyEnemiesAlive() const;
		Hero& getRandomAliveHero();

		// Core game methods
		void playGame();
		void playRound();
		void executeAction(CombatAction action);

		// Turn management
		void heroPhase();
		std::vector<CombatAction> enemyPhase();
		void resolveTurn(const std::vector<CombatAction>& enemyActions);
};

#endif