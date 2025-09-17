#include "Game.hpp"
#include "Display.hpp"
#include <cstdlib>
#include "Input.hpp"
#include "Die.hpp"

Game::Game() : round(1) {}

void Game::addHero(const std::string& name, int health) {
		heroes.push_back(Hero(name, health));
}

void Game ::addEnemy(const std::string& name, int health) {
		enemies.push_back(Enemy(name, health));
}

bool Game::anyHeroesAlive() const {
		for (const auto& hero : heroes) {
				if (hero.isAlive())
						return true;
		}
		return false;
}

bool Game::anyEnemiesAlive() const {
		for (const auto& enemy : enemies) {
				if (enemy.isAlive())
						return true;
		}
		return false;
}

Hero& Game::getRandomAliveHero() {
		std::vector<int> aliveHeroes;
		for (size_t i = 0; i < heroes.size(); ++i) {
				if (heroes[i].isAlive())
						aliveHeroes.push_back(i);
		}
		int randomIndex = aliveHeroes[generator() % aliveHeroes.size()];
		return heroes[randomIndex];
}

bool Game::isGameOver() const {
		return !anyHeroesAlive() || !anyEnemiesAlive();
}

void Game::playGame() {
	while (!isGameOver()) {
			playRound();
	}
	Display::showGameOver(anyHeroesAlive());
}

void Game::playRound() {
	Display::showRoundHeader(round);
	for (auto& hero : heroes) {
			hero.resetShield(); // Reset shields at the start of each round
			hero.setIncomingDamage(0); // Reset incoming damage
			Display::showStatus(hero);
			std::cout << std::endl;
			hero.displayDie();
			std::cout << std::endl << std::endl;
	}
	for (auto& enemy : enemies) {
		if (enemy.isAlive()) {
			enemy.resetShield(); // Reset shields at the start of each round
			enemy.setIncomingDamage(0); // Reset incoming damage
			Display::showStatus(enemy);
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;

	std::vector<CombatAction> enemyRolls = enemyPhase();  // All enemies roll
	heroPhase();    // All heroes roll/reroll and execute

	resolveTurn(enemyRolls);               // Resolve all actions

	round++;
}

void Game::executeAction(CombatAction action) {
		if (action.targetTeam == 1) { // Target is an enemy
				if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < enemies.size() &&
						enemies[action.targetIndex].isAlive()) {
						if (action.roll.action == ATTACK) {
								ActionResult result = enemies[action.targetIndex].takeDamage(action.roll.value);
								Display::showActionResult(action, result);
						}
				}
		} else if (action.targetTeam == 0) { // Target is a hero
				if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < heroes.size() &&
						heroes[action.targetIndex].isAlive()) {
						if (action.roll.action == HEAL) {
								ActionResult result = heroes[action.targetIndex].heal(action.roll.value);
								Display::showActionResult(action, result);
						} else if (action.roll.action == BLOCK) {
								ActionResult result = heroes[action.targetIndex].addShield(action.roll.value);
								Display::showActionResult(action, result);
						}
				}
		}
		return;
}

std::vector<CombatAction> Game::enemyPhase() {
	std::vector<CombatAction> actions;

	for (size_t i = 0; i < enemies.size(); ++i) {
		if (enemies[i].isAlive()) {
			DiceFace roll = enemies[i].roll();

			int targetIndex = -1;
			std::vector<int> aliveHeroes;
			for (size_t i = 0; i < heroes.size(); ++i) {
				if (heroes[i].isAlive()) {
					aliveHeroes.push_back(i);
				}
			}
			targetIndex = aliveHeroes[generator() % aliveHeroes.size()];
			heroes[targetIndex].setIncomingDamage(heroes[targetIndex].getIncomingDamage() + (roll.action == ATTACK ? roll.value : 0));

			CombatAction action(roll, i, 0, targetIndex, enemies[i].getName(), heroes[targetIndex].getName());

			Display::showIntent(action);

			actions.push_back(action);
		}
		else {
			actions.push_back(CombatAction(DiceFace(EMPTY, NONE, 0), i, 1, 0, enemies[i].getName(), "")); // No action for dead enemies
		}
	}
	return actions;
}

void Game::heroPhase() {
		for (size_t i = 0; i < heroes.size(); ++i) {
				if (heroes[i].isAlive() && anyEnemiesAlive()) {
						int rerolls = 2;
						DiceFace roll = heroes[i].roll();
						std::cout << std::endl;
						Display::showDiceRoll(heroes[i].getName(), roll);
						std::cout << std::endl;

						while (rerolls > 0) {
								std::cout << "1. Keep this roll" << std::endl;
								std::cout << "2. Reroll (remaining: " << rerolls << ")" << std::endl;
								int choice = Input::getPlayerChoice(1, 2);

								if (choice == 2) {
									Display::clearLines(4);
									std::cout << std::endl << "Rerolling..." << std::endl << std::endl;
									roll = heroes[i].roll();
									Display::showDiceRoll(heroes[i].getName(), roll);
									rerolls--;
								} else {
									// Display::clearLines(3);
									break; // Keep the roll
								}
								std::cout << std::endl;
						}
						if (roll.target == ENEMY) {
								int targetChoice = Input::getTargetChoice(enemies);
								CombatAction action(roll, i, 1, targetChoice, heroes[i].getName(), enemies[targetChoice].getName());
								executeAction(action);
						} else if (roll.target == ALLY) {
								int targetChoice = Input::getTargetChoice(heroes);
								CombatAction action(roll, i, 0, targetChoice, heroes[i].getName(), heroes[targetChoice].getName());
								executeAction(action);
						} else { // NONE target
								CombatAction action(roll, i, 0, 0, heroes[i].getName(), "");
								executeAction(action);
						}
				}
		}
		return;
}

void Game::resolveTurn(const std::vector<CombatAction>& enemyActions) {
    // Enemies act
    for (const auto& action : enemyActions) {
			// Bounds checking for safety
        if (action.actorIndex < 0 || static_cast<size_t>(action.actorIndex) >= enemies.size()) continue;
        if (!enemies[action.actorIndex].isAlive()) continue;

        Enemy& actor = enemies[action.actorIndex];

        if (action.roll.action == ATTACK && action.roll.target == ENEMY) {
            // Enemy attacking hero (ENEMY target means "attack the enemy team" from enemy perspective)
            if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < heroes.size() &&
                heroes[action.targetIndex].isAlive()) {
								ActionResult result = heroes[action.targetIndex].takeDamage(action.roll.value);
                Display::showActionResult(action, result);
            } else {
                std::cout << actor.getName() << "'s target is no longer available!" << std::endl;
            }
        } else if (action.roll.action == HEAL && action.roll.target == ALLY) {
            // Enemy healing ally (or self)
            if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < enemies.size() &&
                enemies[action.targetIndex].isAlive()) {
								ActionResult result = enemies[action.targetIndex].heal(action.roll.value);
                Display::showActionResult(action, result);
            }
        } else if (action.roll.action == BLOCK && action.roll.target == ALLY) {
            // Enemy shielding ally (or self)
            if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < enemies.size() &&
                enemies[action.targetIndex].isAlive()) {
                if (action.targetIndex == action.actorIndex) {
                    std::cout << actor.getName() << " shields themselves!" << std::endl;
                } else {
                    std::cout << actor.getName() << " shields " << action.targetName << "!" << std::endl;
                }
                enemies[action.targetIndex].addShield(action.roll.value);
            }
        } else {
            std::cout << actor.getName() << " does nothing this turn." << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "--- END OF TURN ---" << std::endl;
    std::cout << std::endl;
}