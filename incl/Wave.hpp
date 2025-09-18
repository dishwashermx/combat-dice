#ifndef WAVE_HPP
# define WAVE_HPP

#include <vector>
#include <iostream>

#include "Game.hpp"

struct CombatAction
{
	DiceFace roll;
	std::string actorName;
	int actorIndex;
	int actorTeam; // 0 for hero, 1 for monster
	std::string targetName;
	int targetIndex;
	int targetTeam; // 0 for hero, 1 for monster

	CombatAction(DiceFace r, std::string aName, int aIdx, int aTeam, std::string tName, int tIdx, int tTeam)
	: roll(r), actorName(aName), actorIndex(aIdx), actorTeam(aTeam), targetName(tName), targetIndex(tIdx), targetTeam(tTeam) {}
};


class Wave {
	private:
		Game& game;
		int round;


	public:
		Wave(Game& gameRef);

		void playWave();
		void heroPhase();
		void executeAction(CombatAction action);
		void recalculateIncomingDamage();
		std::vector<CombatAction> monsterPhase();
		void resolveTurn(const std::vector<CombatAction>& monsterActions);
};

#endif