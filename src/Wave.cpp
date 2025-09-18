#include "Wave.hpp"
#include "Display.hpp"
#include "Input.hpp"

Wave::Wave(Game& gameRef) : game(gameRef), round(1) {}

void Wave::playWave() {
	Display::showRoundHeader(round);
	for (auto& hero : game.heroes) {
			hero.resetShield(); // Reset shields at the start of each round
			hero.setIncomingDamage(0); // Reset incoming damage
			Display::showStatus(hero);
			std::cout << std::endl;
			hero.displayDie();
			std::cout << std::endl;
	}
	std::cout << std::endl;
	for (auto& monster : game.monsters) {
		if (monster.isAlive()) {
			monster.resetShield(); // Reset shields at the start of each round
			monster.setIncomingDamage(0); // Reset incoming damage
			Display::showStatus(monster);
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;

	Input::pressEnterToContinue();

	std::vector<CombatAction> monsterRolls = monsterPhase();  // All monsters roll
	heroPhase();    // All heroes roll/reroll and execute

	resolveTurn(monsterRolls);               // Resolve all actions

	round++;
}

void Wave::heroPhase() {
		for (size_t i = 0; i < game.heroes.size(); ++i) {
				if (game.heroes[i].isAlive() && game.anyMonstersAlive()) {
						int rerolls = 2;
						DiceFace roll = Display::animatedRoll(game.heroes[i]);
						std::cout << std::endl;

						while (rerolls > 0) {
								std::cout << "1. Keep this roll" << std::endl;
								std::cout << "2. Reroll (remaining: " << rerolls << ")" << std::endl;
								int choice = Input::getPlayerChoice(1, 2);

								if (choice == 2) {
									Display::clearLines(4);
									std::cout << "Rerolling..." << std::endl << std::endl;
									roll = Display::animatedRoll(game.heroes[i]);
									rerolls--;
								} else {
									// Display::clearLines(3);
									break; // Keep the roll
								}
								std::cout << std::endl;
						}
						if (roll.target == ENEMY) {
								int targetChoice = Input::getTargetChoice(game.monsters);
								CombatAction action(roll, game.heroes[i].getName(), i, 0, game.monsters[targetChoice].getName(), targetChoice, 1);
								executeAction(action);
						} else if (roll.target == ALLY) {
								int targetChoice = Input::getTargetChoice(game.heroes);
								CombatAction action(roll, game.heroes[i].getName(), i, 0, game.heroes[targetChoice].getName(), targetChoice, 0);
								executeAction(action);
						} else { // NONE target
								CombatAction action(roll, game.heroes[i].getName(), i, 0, "", -1, 0);
								executeAction(action);
						}
				}
		}
		return;
}



void Wave::executeAction(CombatAction action) {
		if (action.roll.action == EMPTY) {
				ActionResult result = {0, 0, 0, 0, 0, 0, 0, 0, 0};
				Display::showActionResult(action, result);
				return;
		}
		else if (action.targetTeam == 1) { // Target is an monster
				if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < game.monsters.size()) {
						if (action.roll.action == ATTACK) {
								ActionResult result = game.monsters[action.targetIndex].takeDamage(action.roll.value);
								Display::showActionResult(action, result);
						}
				}
		} else if (action.targetTeam == 0) { // Target is a hero
				if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < game.heroes.size()) {
						if (action.roll.action == HEAL) {
								ActionResult result = game.heroes[action.targetIndex].heal(action.roll.value);
								Display::showActionResult(action, result);
						} else if (action.roll.action == BLOCK) {
								ActionResult result = game.heroes[action.targetIndex].addShield(action.roll.value);
								Display::showActionResult(action, result);
						}
				}
		}

		for (auto& monster : game.monsters) {
				if (!monster.isAlive() && monster.getRoundOfDeath() != -1) {
						monster.setRoundOfDeath(round); // Mark the round of death
				}
				if (monster.getRoundOfDeath() == round) {
						recalculateIncomingDamage();
				}
		}
		return;
}

void Wave::recalculateIncomingDamage() {
		for (auto& hero : game.heroes) {
				hero.setIncomingDamage(0);
		}
		for (const auto& action : monsterPhase()) {
				if (action.roll.action == ATTACK && action.targetTeam == 0) { // Targeting heroes
						if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < game.heroes.size()) {
								game.heroes[action.targetIndex].setIncomingDamage(game.heroes[action.targetIndex].getIncomingDamage() + action.roll.value);
						}
				}
		}
}

std::vector<CombatAction> Wave::monsterPhase() {
	std::vector<CombatAction> actions;

	for (size_t i = 0; i < game.monsters.size(); ++i) {
		if (game.monsters[i].isAlive()) {
			DiceFace roll = game.monsters[i].roll();

			int targetIndex = -1;
			std::vector<int> aliveHeroes;
			for (size_t i = 0; i < game.heroes.size(); ++i) {
				if (game.heroes[i].isAlive()) {
					aliveHeroes.push_back(i);
				}
			}
			targetIndex = aliveHeroes[game.generator() % aliveHeroes.size()];
			game.heroes[targetIndex].setIncomingDamage(game.heroes[targetIndex].getIncomingDamage() + (roll.action == ATTACK ? roll.value : 0));

			CombatAction action(roll, game.monsters[i].getName(), i, 1, game.heroes[targetIndex].getName(), targetIndex, 0);

			Display::showIntent(action);
			std::cout << std::endl;

			actions.push_back(action);
		}
		else {
			actions.push_back(CombatAction(DiceFace(EMPTY, NONE, 0), game.monsters[i].getName(), i, 1, "", -1, 0)); // No action for dead monsters
		}
	}
	std::cout << std::endl;
	return actions;
}

void Wave::resolveTurn(const std::vector<CombatAction>& monsterActions) {
    // Monsters act
    for (const auto& action : monsterActions) {
			// Bounds checking for safety
        if (action.actorIndex < 0 || static_cast<size_t>(action.actorIndex) >= game.monsters.size()) continue;
        if (!game.monsters[action.actorIndex].isAlive()) continue;

        Character& actor = game.monsters[action.actorIndex];

        if (action.roll.action == ATTACK && action.roll.target == ENEMY) {
            // Monster attacking hero (ENEMY target means "attack the monster team" from monster perspective)
            if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < game.heroes.size() &&
                game.heroes[action.targetIndex].isAlive()) {
								ActionResult result = game.heroes[action.targetIndex].takeDamage(action.roll.value);
                Display::showActionResult(action, result);
            } else {
                std::cout << actor.getName() << "'s target is no longer available!" << std::endl;
            }
        } else if (action.roll.action == HEAL && action.roll.target == ALLY) {
            // Monster healing ally (or self)
            if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < game.monsters.size() &&
                game.monsters[action.targetIndex].isAlive()) {
                ActionResult result = game.monsters[action.targetIndex].heal(action.roll.value);
                Display::showActionResult(action, result);
            }
        } else if (action.roll.action == BLOCK && action.roll.target == ALLY) {
            // Monster shielding ally (or self)
            if (action.targetIndex >= 0 && static_cast<size_t>(action.targetIndex) < game.monsters.size() &&
                game.monsters[action.targetIndex].isAlive()) {
                if (action.targetIndex == action.actorIndex) {
                    std::cout << actor.getName() << " shields themselves!" << std::endl;
                } else {
                    std::cout << actor.getName() << " shields " << action.targetName << "!" << std::endl;
                }
                game.monsters[action.targetIndex].addShield(action.roll.value);
            }
        } else {
            std::cout << actor.getName() << " does nothing this turn." << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "--- END OF TURN ---" << std::endl;
    std::cout << std::endl;
}
