#include "Wave.hpp"
#include "Display.hpp"
#include "Input.hpp"

Wave::Wave(Game& gameRef) : game(gameRef), round(1) {}

void Wave::setupWave() {
	switch (game.waveNumber) {
		case 1:
			game.monsters.push_back(CharacterFactory::createGoblin());
			game.monsters.push_back(CharacterFactory::createGoblin());

			break;
		case 2:
			game.monsters.push_back(CharacterFactory::createGoblin());
			// game.monsters.push_back(CharacterFactory::createOrc());
			break;
		case 3:
			game.monsters.push_back(CharacterFactory::createGoblin());
			// game.monsters.push_back(CharacterFactory::createOrc());
			// game.monsters.push_back(CharacterFactory::createOrc());
			break;
		default:
			game.addMonster();
			break;
	}
}

void Wave::playWave() {
	Display::showWaveHeader(game.waveNumber);

	// Helper lambda for character setup and display
	auto setupCharacter = [](Character& character, bool isHero) {
		character.resetShield();
		character.setIncomingDamage(0);
		if (isHero) {
			if (character.isAlive())
				character.setHealth(character.getMaxHealth());
			else
				character.setHealth(character.getMaxHealth()/2); // Revive with half health
		}
	};

	std::cout << "HEROES" << std::endl;
	for (auto& hero : game.heroes) {
		setupCharacter(hero, true);
		Display::showStatus(hero);
		std::cout << std::endl;
		hero.displayDie();
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "MONSTERS" << std::endl;
	for (auto& monster : game.monsters) {
		if (monster.isAlive()) {
			setupCharacter(monster, false);
			Display::showStatus(monster);
			std::cout << std::endl;
			monster.displayDie();
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;

	Input::pressEnterToContinue();

}

void Wave::playRound() {
    Display::showRoundHeader(round);

    // Reset all characters and display status
    auto resetAndShowCharacter = [](Character& character) {
        character.resetShield();
        character.setIncomingDamage(0);
        character.resetDodge();
        character.resetStun();
        Display::showStatus(character);
        std::cout << std::endl;
    };

    for (auto& hero : game.heroes) {
        resetAndShowCharacter(hero);
    }
    std::cout << std::endl;
    for (auto& monster : game.monsters) {
        resetAndShowCharacter(monster);
    }
		std::cout << std::endl;
    currentMonsterActions = monsterPhase();
		Input::pressEnterToContinue();
		std::cout << std::endl;
    heroPhase();
    // Recalculate incoming damage after heroes have had chance to dodge
    recalculateIncomingDamage(currentMonsterActions);
    resolveTurn(currentMonsterActions);

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
                    break; // Keep the roll
                }
                std::cout << std::endl;
            }

            // Create action based on action type and marks
            CombatAction action(roll, game.heroes[i].getName(), i, 0, "", -1, 0);

            if (roll.action == ATTACK || roll.action == STUN) {
                // Offensive actions - check if we need to select a target
                if (roll.marks == WAVE || roll.marks == QUAKE) {
                    // Area attacks - no target selection needed
                    action = CombatAction(roll, game.heroes[i].getName(), i, 1, "Multiple", -1, -1);
                } else {
                    // Single/adjacent target needed
                    int targetChoice = Input::getTargetChoice(game.monsters);
                    action = CombatAction(roll, game.heroes[i].getName(), i, 1, game.monsters[targetChoice].getName(), targetChoice, -1);
                }
            }
            else if (roll.action == HEAL || roll.action == BLOCK) {
                // Support actions - check if we need to select a target
                if (roll.marks == WAVE || roll.marks == QUAKE) {
                    // Area support - no target selection needed
                    action = CombatAction(roll, game.heroes[i].getName(), i, 1, "Multiple", -1, 1);
                } else {
                    // Single/adjacent target needed
                    int targetChoice = Input::getTargetChoice(game.heroes);
                    action = CombatAction(roll, game.heroes[i].getName(), i, 1, game.heroes[targetChoice].getName(), targetChoice, 1);
                }
            }
            else if (roll.action == DODGE) {
                // Dodge - always self-target, no selection needed
                action = CombatAction(roll, game.heroes[i].getName(), i, 1, game.heroes[i].getName(), i, 1);
            }
						else { // EMPTY or other actions
								action = CombatAction(roll, game.heroes[i].getName(), i, 1, "", -1, 0);
						}

						executeAction(action);

						// If this was a dodge or stun action, recalculate incoming damage immediately
						if (roll.action == DODGE || roll.action == STUN) {
							recalculateIncomingDamage(currentMonsterActions);
						}
				}
    }
    return;
}

std::vector<Character*> Wave::getTargets(const CombatAction& action, Character*) {
    std::vector<Character*> targets;

    // Determine which team to target based on action type
    std::vector<Character>* targetTeam = nullptr;

    if (action.roll.action == ATTACK || action.roll.action == STUN) {
        // Offensive actions target enemies
        if (action.targetTeam == -1) { // Hero targeting monsters
            targetTeam = &game.monsters;
        } else { // Monster targeting heroes
            targetTeam = &game.heroes;
        }
    } else if (action.roll.action == HEAL || action.roll.action == BLOCK || action.roll.action == DODGE) {
        // Defensive/support actions target allies (including dodge on self)
        if (action.targetTeam == 1) { // Hero targeting heroes
            targetTeam = &game.heroes;
        } else { // Monster targeting monsters
            targetTeam = &game.monsters;
        }
    } else if (action.roll.action == EMPTY) {
        // Empty actions don't need targets
        return targets;
    }

    // Apply mark-based targeting
    switch(action.roll.marks) {
        case SPLASH: {
            // Adjacent characters (target + neighbors)
            if (action.targetIndex >= 0 && action.targetIndex < (int)targetTeam->size()) {
                for (int offset = -1; offset <= 1; ++offset) {
                    int index = action.targetIndex + offset;
                    if (index >= 0 && index < (int)targetTeam->size() && (*targetTeam)[index].isAlive()) {
                        targets.push_back(&(*targetTeam)[index]);
                    }
                }
            }
            break;
        }
        case WAVE: {
            // Target entire team
            for (auto& character : *targetTeam) {
                if (character.isAlive()) {
                    targets.push_back(&character);
                }
            }
            break;
        }
        case QUAKE: {
            // Everyone alive (both teams)
            for (auto& hero : game.heroes) {
                if (hero.isAlive()) {
                    targets.push_back(&hero);
                }
            }
            for (auto& monster : game.monsters) {
                if (monster.isAlive()) {
                    targets.push_back(&monster);
                }
            }
            break;
        }
        default: {
            if (action.targetIndex >= 0 && action.targetIndex < (int)targetTeam->size() &&
                (*targetTeam)[action.targetIndex].isAlive()) {
                targets.push_back(&(*targetTeam)[action.targetIndex]);
            }
            break;
        }
    }

    return targets;
}

void Wave::executeAction(CombatAction action) {
    // Get the actor
    Character* actor = nullptr;
    if (action.actorTeam == 1) { // Hero
        if (action.actorIndex >= 0 && action.actorIndex < (int)game.heroes.size()) {
            actor = &game.heroes[action.actorIndex];
        }
    } else { // Monster
        if (action.actorIndex >= 0 && action.actorIndex < (int)game.monsters.size()) {
            actor = &game.monsters[action.actorIndex];
        }
    }

    if (!actor || !actor->isAlive()) return;

    // Skip action entirely if actor is stunned
    if (actor->isStunned()) {
        return;
    }

    // Get targets based on target type
    std::vector<Character*> targets = getTargets(action, actor);

    // Execute action on all targets
    for (Character* target : targets) {
        if (!target || !target->isAlive()) continue;

        // Create individual action for each target
        CombatAction individualAction = action;
        individualAction.targetName = target->getName();

        // Find target index
        if (action.targetTeam == 1) { // Hero target
            for (size_t i = 0; i < game.heroes.size(); ++i) {
                if (&game.heroes[i] == target) {
                    individualAction.targetIndex = i;
                    break;
                }
            }
        } else { // Monster target
            for (size_t i = 0; i < game.monsters.size(); ++i) {
                if (&game.monsters[i] == target) {
                    individualAction.targetIndex = i;
                    break;
                }
            }
        }

        ActionResult result;
        switch(action.roll.action) {
            case ATTACK:
                result = target->takeDamage(action.roll.value);
                Display::showActionResult(individualAction, result);
                break;

            case HEAL:
                result = target->heal(action.roll.value);
                Display::showActionResult(individualAction, result);
                break;

            case BLOCK:
                result = target->addShield(action.roll.value);
                Display::showActionResult(individualAction, result);
                break;

            case DODGE:
                result = target->dodge();
                Display::showActionResult(individualAction, result);
                break;

            case STUN:
                result = target->stun(actor->getHealth());
                if (result.wasDodged || result.wasStunned) {
                    Display::showActionResult(individualAction, result);
                }
                break;

            case EMPTY:
                result = {0, 0, 0, 0, 0, 0, 0, 0, 0, false, false};
                Display::showActionResult(individualAction, result);
                break;
        }
			std::cout << std::endl;
    }

    // Handle monster deaths
    for (auto& monster : game.monsters) {
        if (!monster.isAlive() && monster.getRoundOfDeath() != -1) {
            monster.setRoundOfDeath(round);
        }
        // Note: incoming damage will be recalculated after hero phase
    }
}

void Wave::recalculateIncomingDamage(const std::vector<CombatAction>& monsterActions) {
		// Reset all hero incoming damage
		for (auto& hero : game.heroes) {
				hero.setIncomingDamage(0);
		}

		// Process monster attacks
		for (const auto& action : monsterActions) {
				if (action.roll.action != ATTACK || action.targetTeam != 1) continue; // Skip non-attacks or non-hero targets

				// Bounds check once
				if (action.targetIndex < 0 || action.targetIndex >= static_cast<int>(game.heroes.size()) ||
				    action.actorIndex < 0 || action.actorIndex >= static_cast<int>(game.monsters.size())) {
						continue;
				}

				// Check conditions once
				if (game.heroes[action.targetIndex].isDodging() || game.monsters[action.actorIndex].isStunned()) {
						continue;
				}

				// Apply damage
				auto& targetHero = game.heroes[action.targetIndex];
				targetHero.setIncomingDamage(targetHero.getIncomingDamage() + action.roll.value);
		}
}

std::vector<CombatAction> Wave::monsterPhase() {
    std::vector<CombatAction> actions;

    for (size_t i = 0; i < game.monsters.size(); ++i) {
        if (game.monsters[i].isAlive()) {
            DiceFace roll = game.monsters[i].roll();

            // Create action based on action type and marks
            CombatAction action(roll, game.monsters[i].getName(), i, -1, "", -1, 0);

            if (roll.action == ATTACK || roll.action == STUN) {
                // Offensive actions - check if we need to select a target
                if (roll.marks == WAVE || roll.marks == QUAKE) {
                    // Area attacks - no target selection needed
                    action = CombatAction(roll, game.monsters[i].getName(), i, -1, "Multiple", 1, 0);
                } else {
                    // Single/adjacent target needed - pick random hero
                    std::vector<int> aliveHeroes;
                    aliveHeroes.reserve(game.heroes.size()); // Reserve space to avoid reallocations
                    for (size_t j = 0; j < game.heroes.size(); ++j) {
                        if (game.heroes[j].isAlive()) {
                            aliveHeroes.push_back(static_cast<int>(j));
                        }
                    }

                    if (!aliveHeroes.empty()) {
                        const int randomIndex = game.generator() % aliveHeroes.size();
                        const int targetIndex = aliveHeroes[randomIndex];
                        const auto& targetHero = game.heroes[targetIndex];
                        action = CombatAction(roll, game.monsters[i].getName(), i, -1, targetHero.getName(), targetIndex, 1);

                        // For damage prediction - calculate for all affected targets
                        if (roll.action == ATTACK) {
                            // Use the same targeting logic as executeAction to calculate damage
                            if (roll.marks == SPLASH) {
                                // Adjacent characters (target + neighbors)
                                for (int offset = -1; offset <= 1; ++offset) {
                                    int splashIndex = targetIndex + offset;
                                    if (splashIndex >= 0 && splashIndex < static_cast<int>(game.heroes.size()) &&
                                        game.heroes[splashIndex].isAlive() && !game.heroes[splashIndex].isDodging()) {
                                        game.heroes[splashIndex].setIncomingDamage(
                                            game.heroes[splashIndex].getIncomingDamage() + roll.value);
                                    }
                                }
                            } else if (roll.marks == WAVE) {
                                // WAVE - affects entire target team (all heroes)
                                for (auto& hero : game.heroes) {
                                    if (hero.isAlive() && !hero.isDodging()) {
                                        hero.setIncomingDamage(hero.getIncomingDamage() + roll.value);
                                    }
                                }
                            } else if (roll.marks == QUAKE) {
                                // QUAKE - affects everyone alive (heroes and monsters)
                                for (auto& hero : game.heroes) {
                                    if (hero.isAlive() && !hero.isDodging()) {
                                        hero.setIncomingDamage(hero.getIncomingDamage() + roll.value);
                                    }
                                }
                            } else {
                                // Single target only
                                if (!targetHero.isDodging()) {
                                    game.heroes[targetIndex].setIncomingDamage(
                                        targetHero.getIncomingDamage() + roll.value);
                                }
                            }
                        }
                    } else {
                        action = CombatAction(roll, game.monsters[i].getName(), i, -1, "", -1, 0);
                    }
                }
            } else if (roll.action == HEAL || roll.action == BLOCK) {
                // Support actions - check if we need to select a target
                if (roll.marks == WAVE || roll.marks == QUAKE) {
                    // Area support - no target selection needed
                    action = CombatAction(roll, game.monsters[i].getName(), i, -1, "Multiple", -1, -1);
                } else {
                    // Single/adjacent target needed - pick random monster
                    std::vector<int> aliveMonsters;
                    for (size_t j = 0; j < game.monsters.size(); ++j) {
                        if (game.monsters[j].isAlive()) {
                            aliveMonsters.push_back(j);
                        }
                    }

                    if (!aliveMonsters.empty()) {
                        int randomIndex = game.generator() % aliveMonsters.size();
                        int targetIndex = aliveMonsters[randomIndex];
                        action = CombatAction(roll, game.monsters[i].getName(), i, -1, game.monsters[targetIndex].getName(), targetIndex, 1);
                    } else {
                        action = CombatAction(roll, game.monsters[i].getName(), i, -1, "", -1, -1);
                    }
                }
            } else { // EMPTY or other actions
                action = CombatAction(DiceFace(), game.monsters[i].getName(), i, -1, "", -1, 0);
            }

            Display::showIntent(action);
            std::cout << std::endl;
            actions.push_back(action);
        }
        else {
            actions.push_back(CombatAction(DiceFace(), game.monsters[i].getName(), i, 1, "", -1, 0)); // No action for dead monsters
        }
    }
    std::cout << std::endl;
    return actions;
}

void Wave::resolveTurn(const std::vector<CombatAction>& monsterActions) {
    // Execute monster actions using the unified executeAction method
    for (const auto& action : monsterActions) {
        // Bounds checking for safety
        if (action.actorIndex < 0 || static_cast<size_t>(action.actorIndex) >= game.monsters.size()) continue;
        if (!game.monsters[action.actorIndex].isAlive()) continue;

        executeAction(action);
    }
}

bool Wave::isRoundOver() const {
		return !game.anyHeroesAlive() || !game.anyMonstersAlive();
}