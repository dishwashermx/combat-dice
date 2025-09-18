#include "Game.hpp"
#include "Wave.hpp"

Game::Game() {}

void Game::playGame() {
	mainMenu();
	partySetup();
	while (wave < 3 && anyHeroesAlive()) {
			Wave wave(*this);
			wave.playWave();
	}
	Display::showGameOver(anyHeroesAlive());
}

void Game::addHero() {
		heroes.push_back(CharacterFactory::createKnight());
}

void Game::addMonster() {
		monsters.push_back(CharacterFactory::createGoblin());
}

bool Game::anyHeroesAlive() const {
		for (const auto& hero : heroes) {
				if (hero.isAlive())
						return true;
		}
		return false;
}

bool Game::anyMonstersAlive() const {
		for (const auto& monster : monsters) {
				if (monster.isAlive())
						return true;
		}
		return false;
}

Character& Game::getRandomAliveHero() {
		std::vector<int> aliveHeroes;
		for (size_t i = 0; i < heroes.size(); ++i) {
				if (heroes[i].isAlive())
						aliveHeroes.push_back(i);
		}
		int randomIndex = aliveHeroes[generator() % aliveHeroes.size()];
		return heroes[randomIndex];
}

bool Game::isGameOver() const {
		return !anyHeroesAlive() || !anyMonstersAlive();
}
