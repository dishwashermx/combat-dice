#include "Game.hpp"
#include "Wave.hpp"

Game::Game() : waveNumber{1} {}

void Game::playGame() {
	mainMenu();
	partySetup();
	while (waveNumber < 4 && anyHeroesAlive()) {
		Wave wave(*this);
		wave.setupWave();
		wave.playWave();
		while (anyHeroesAlive() && anyMonstersAlive()) {
			wave.playRound();
			if (wave.isRoundOver())
			break;
		}
		monsters.clear();
		waveNumber++;
	}
	Display::showGameOver(anyHeroesAlive());
}

void Game::mainMenu() {
		Display::titleScreen();
    Input::pressEnterToContinue("");
	return;
}

void Game::partySetup() {
	heroes.push_back(CharacterFactory::createKnight());
	// heroes.push_back(CharacterFactory::createAssassin());
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
