#include "CharacterFactory.hpp"


// ===== HEROES =====
Character CharacterFactory::createKnight() {
    std::vector<DiceFace> dice = {
        DiceFace(BLOCK, ALLY, 3),
        DiceFace(BLOCK, ALLY, 3),
        DiceFace(BLOCK, ALLY, 3),
        DiceFace(BLOCK, ALLY, 3),
        DiceFace(BLOCK, ALLY, 3),
        DiceFace(BLOCK, ALLY, 3)
    };
    return Character("Knight", 6, dice, "Hero");
}

// ===== MONSTERS =====
Character CharacterFactory::createGoblin() {
    std::vector<DiceFace> dice = {
        DiceFace(ATTACK, ENEMY, 2),
        DiceFace(ATTACK, ENEMY, 2),
        DiceFace(ATTACK, ENEMY, 2),
        DiceFace(ATTACK, ENEMY, 2),
        DiceFace(ATTACK, ENEMY, 2),
        DiceFace(ATTACK, ENEMY, 2),
    };
    return Character("Goblin", 4, dice, "Monster");
}