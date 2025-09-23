#include "CharacterFactory.hpp"


// ===== HEROES =====
Character CharacterFactory::createKnight() {
    std::vector<DiceFace> dice = {
        DiceFace::createBlock("Shield Wall", 3),
        DiceFace::createBlock("Shield Bash", 2),
        DiceFace::createAttack("Sword Strike", 2),
        DiceFace::createAttack("Power Attack", 3),
        DiceFace::createBlock("Protect Allies", 2, WAVE),  // Shield whole team
        DiceFace::createAttack("Cleave", 2, SPLASH)          // Hit adjacent enemies
    };

    return Character("Knight", 6, dice, "Hero");
}

// Character CharacterFactory::createAssassin() {
//     std::vector<DiceFace> dice = {
//         DiceFace(ATTACK, 2),
//         DiceFace(ATTACK, 2),
//         DiceFace(ATTACK, 1),
//         DiceFace(HEAL, 1),
//         DiceFace(EMPTY, 0),
//         DiceFace(EMPTY, 0)
//     };
//     return Character("Assassin", 4, dice, "Hero");
// }

// ===== MONSTERS =====
Character CharacterFactory::createGoblin() {
    std::vector<DiceFace> dice = {
        DiceFace("Goblin Slash", "slashes", "🗡️", ATTACK, 2, NO_MARK),
        DiceFace("Goblin Slash", "slashes", "🗡️", ATTACK, 2, NO_MARK),
        DiceFace("Goblin Slash", "slashes", "🗡️", ATTACK, 2, NO_MARK),
        DiceFace("Goblin Slash", "slashes", "🗡️", ATTACK, 2, NO_MARK),
        DiceFace("Goblin Slash", "slashes", "🗡️", ATTACK, 2, NO_MARK),
        DiceFace("Goblin Slash", "slashes", "🗡️", ATTACK, 2, NO_MARK),
    };
    return Character("Goblin", 4, dice, "Monster");
}

// Character CharacterFactory::createOrc() {
//     std::vector<DiceFace> dice = {
//         DiceFace(ATTACK, 3),
//         DiceFace(ATTACK, 3),
//         DiceFace(ATTACK, 2),
//         DiceFace(ATTACK, 2),
//         DiceFace(ATTACK, 1),
//         DiceFace(EMPTY, 0)
//     };
//     return Character("Orc", 8, dice, "Monster");
// }