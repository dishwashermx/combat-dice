// characters/CharacterFactory.hpp
#ifndef CHARACTER_FACTORY_HPP
#define CHARACTER_FACTORY_HPP

#include "Character.hpp"

namespace CharacterFactory {
	// Heroes
	Character createKnight();
	Character createAssassin();

	// Monsters
	Character createGoblin();
	// Character createOrc();
    // HEROES (20 types)
    // Tank-type heroes
    // Character createKnight();
    // Character createPaladin();
    // Character createGuardian();
    // Character createCrusader();

    // // DPS-type heroes
    // Character createBerserker();
    // Character createArcher();
    // Character createRanger();
    // Character createWarrior();

    // // Magic-type heroes
    // Character createWizard();
    // Character createSorcerer();
    // Character createElementalist();
    // Character createNecromancer();

    // // Support-type heroes
    // Character createCleric();
    // Character createPriest();
    // Character createDruid();
    // Character createBard();

    // // Hybrid heroes
    // Character createPaladin();
    // Character createSpellsword();
    // Character createMonk();

    // // MONSTERS (20 types)
    // // Weak monsters
    // Character createGoblin();
    // Character createSkeleton();
    // Character createRat();
    // Character createSpider();
    // Character createSlime();

    // // Medium monsters
    // Character createWolf();
    // Character createZombie();
    // Character createBandit();
    // Character createCultist();

    // // Strong monsters
    // Character createTroll();
    // Character createOgre();
    // Character createMinotaur();
    // Character createWyvern();
    // Character createDemon();

    // // Boss monsters
    // Character createDragon();
    // Character createLich();
    // Character createBalrog();
    // Character createArchdemon();
    // Character createAncientEvil();

    // // Utility functions
    // Character createRandomHero();
    // Character createRandomMonster();
    // std::vector<std::string> getAllHeroTypes();
    // std::vector<std::string> getAllMonsterTypes();
}

#endif