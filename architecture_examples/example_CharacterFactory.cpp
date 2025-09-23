// Example CharacterFactory.cpp using the ActionLibrary system
#include "CharacterFactory.hpp"
#include "ActionLibrary.hpp"

// ===== HEROES =====
Character CharacterFactory::createKnight() {
    // Using the Action Library for predefined knight abilities
    std::vector<DiceFace> dice = ActionLibrary::createDiceSet({
        "shield_up",        // Defensive stance
        "sword_slash",      // Basic attack
        "cleave",          // Area attack
        "heavy_strike",    // Power attack
        "armor_buff",      // Protect allies
        "shield_up"        // More defense
    });
    return Character("Knight", 6, dice, "Hero");
}

Character CharacterFactory::createMage() {
    // Spellcaster with area damage and utility
    std::vector<DiceFace> dice = ActionLibrary::createDiceSet({
        "fireball",        // Area damage
        "lightning_bolt",  // Chain damage
        "ice_storm",       // Area freeze damage
        "healing_wave",    // Group heal
        "mana_shield",     // Magical protection
        "focus"           // Concentrate power
    });
    return Character("Mage", 4, dice, "Hero");
}

Character CharacterFactory::createAssassin() {
    // Using the Action Library for predefined assassin abilities
    std::vector<DiceFace> dice = ActionLibrary::createDiceSet({
        "backstab",        // High damage stealth attack
        "poison_blade",    // Toxic strike
        "shadow_strike",   // Quick attack
        "vampiric_drain",  // Life steal
        "stealth",         // Hide
        "preparation"      // Setup next move
    });
    return Character("Assassin", 3, dice, "Hero");
}

Character CharacterFactory::createPaladin() {
    // Holy warrior with balance of offense and support
    std::vector<DiceFace> dice = ActionLibrary::createDiceSet({
        "holy_strike",         // Divine attack
        "lay_on_hands",        // Healing touch
        "divine_protection",   // Group shield
        "smite",              // Powerful divine attack
        "healing_wave",       // Group heal
        "sanctuary"           // Strong group protection
    });
    return Character("Paladin", 5, dice, "Hero");
}

// Alternative: Using the CharacterBuilds templates
Character CharacterFactory::createWarrior() {
    std::vector<DiceFace> dice = ActionLibrary::createDiceSet(CharacterBuilds::WARRIOR());
    return Character("Warrior", 7, dice, "Hero");
}

Character CharacterFactory::createCleric() {
    std::vector<DiceFace> dice = ActionLibrary::createDiceSet(CharacterBuilds::CLERIC());
    return Character("Cleric", 4, dice, "Hero");
}

// ===== MONSTERS =====
Character CharacterFactory::createGoblin() {
    // Monsters can also use predefined actions or custom ones
    std::vector<DiceFace> dice = {
        ActionLibrary::getDiceFace("sword_slash"),  // Mix of library actions
        DiceFace(ATTACK, HERO, 1),                  // and custom actions
        DiceFace(ATTACK, HERO, 2),
        DiceFace(ATTACK, HERO, 2),
        DiceFace(EMPTY, NONE, 0),
        DiceFace(EMPTY, NONE, 0),
    };
    return Character("Goblin", 4, dice, "Monster");
}