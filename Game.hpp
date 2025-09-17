#ifndef GAME_HPP
# define GAME_HPP

#include <vector>
#include <random>
#include <iostream>
#include <limits.h>
#include "Die.hpp"
#include "Hero.hpp"
#include "Enemy.hpp"
#include "Display.hpp"
#include "Input.hpp"

struct CombatAction
{
	DiceFace roll;
	std::string actorName;
	int actorIndex;
	int actorTeam; // 0 for hero, 1 for enemy
	std::string targetName;
	int targetIndex;
	int targetTeam; // 0 for hero, 1 for enemy

	CombatAction(DiceFace r, std::string aName, int aIdx, int aTeam, std::string tName, int tIdx, int tTeam)
	: roll(r), actorName(aName), actorIndex(aIdx), actorTeam(aTeam), targetName(tName), targetIndex(tIdx), targetTeam(tTeam) {}
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
		void recalculateIncomingDamage();

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