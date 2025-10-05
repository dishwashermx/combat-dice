#include "CharacterFactory.hpp"
#include "ActionLibrary.hpp"


// ===== HEROES =====
Character CharacterFactory::createKnight() {
    std::vector<DiceFace> dice = {
        ActionLibrary::getAction("unarmed-strike"),
				ActionLibrary::getAction("sword-slash"),
				ActionLibrary::getAction("sword-slash"),
				ActionLibrary::getAction("buckler"),
				ActionLibrary::getAction("buckler"),
				ActionLibrary::getAction("raise-arms")
    };

    return Character("Knight", 6, dice, "Hero");
}

Character CharacterFactory::createAssassin() {
    std::vector<DiceFace> dice = {
        ActionLibrary::getAction("unarmed-strike"),
				ActionLibrary::getAction("unarmed-strike"),
				ActionLibrary::getAction("unarmed-strike"),
				ActionLibrary::getAction("unarmed-strike"),
				ActionLibrary::getAction("unarmed-strike"),
				ActionLibrary::getAction("unarmed-strike")
    };

    return Character("Assassin", 4, dice, "Hero");
}

// ===== MONSTERS =====
Character CharacterFactory::createGoblin() {
    std::vector<DiceFace> dice = {
        ActionLibrary::getAction("claw-attack"),
        ActionLibrary::getAction("claw-attack"),
        ActionLibrary::getAction("claw-attack"),
        ActionLibrary::getAction("claw-attack"),
				ActionLibrary::getAction("claw-attack"),
				ActionLibrary::getAction("empty")
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