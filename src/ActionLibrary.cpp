#include "ActionLibrary.hpp"
#include <iostream>
#include <iomanip>

std::unordered_map<std::string, DiceFace> ActionLibrary::actions;
bool ActionLibrary::initialized = false;

void ActionLibrary::initialize() {
	if (initialized)
		return;

	// === UNARMED ACTIONS ===
	actions["quick-jab"] = DiceFace("Quick Jab", "jabs", "👊", ATTACK, 1);
	actions["power-cross"] = DiceFace("Power Cross", "crosses", "👊", ATTACK, 2);
	actions["uppercut"] = DiceFace("Uppercut", "uppercuts", "✊", ATTACK, 3);
	actions["roundhouse"] = DiceFace("Roundhouse", "roundhouses", "🦵", ATTACK, 1, SPLASH);
	actions["front-kick"] = DiceFace("Front Kick", "kicks", "🦵", ATTACK, 2);
	actions["brace"] = DiceFace("Brace", "braces", "🛡️", BLOCK, 1);
	actions["duck"] = DiceFace("Duck", "ducks", "🦆", DODGE, 0);
	actions["sidestep"] = DiceFace("Sidestep", "sidesteps", "↔️", DODGE, 0);
	actions["rest"] = DiceFace("Rest", "rests", "😴", HEAL, 1);
	actions["focus"] = DiceFace("Focus", "focuses", "🎯", HEAL, 2);

	// === WEAPON ACTIONS ===
	// Melee weapons (Sword, Axe, Mace, etc.)
	actions["parry"] = DiceFace("Parry", "parries", "⚔️", BLOCK, 2);
	actions["slash"] = DiceFace("Slash", "slashes", "⚔️", ATTACK, 2);
	actions["thrust"] = DiceFace("Thrust", "thrusts", "🗡️", ATTACK, 2);

	// Claw/Natural weapons (Goblin, Beast, Monster)
	actions["scratch"] = DiceFace("Scratch", "scratches", "🪝", ATTACK, 1);
	actions["swipe"] = DiceFace("Swipe", "swipes", "👐", ATTACK, 1, SPLASH);

	// Ranged weapons (Archer, Ranger, etc.)
	actions["aimed-shot"] = DiceFace("Aimed Shot", "shoots", "🎯", ATTACK, 2);
	actions["quick-shot"] = DiceFace("Quick Shot", "shoots", "🏹", ATTACK, 1);

	// === SPECIAL ACTIONS ===
	actions["dodge"] = DiceFace("Dodge", "dodges", "💨", DODGE, 0);
	actions["headbutt"] = DiceFace("Headbutt", "headbutts", "💫", STUN, 0);
	actions["empty"] = DiceFace("Empty", "No action", "❌", EMPTY, 0);

	initialized = true;
}

DiceFace ActionLibrary::getAction(const std::string& name) {
    initialize();

    auto it = actions.find(name);
    if (it != actions.end()) {
        return it->second;
    }

    std::cerr << "Warning: Action '" << name << "' not found!" << std::endl;
    return DiceFace("Unknown", "No action", "❓", EMPTY, 0);
}

std::vector<DiceFace> ActionLibrary::createDiceSet(const std::vector<std::string>& actionNames) {
	std::vector<DiceFace> dice;
	for (const std::string& name : actionNames) {
		dice.push_back(getAction(name));
	}
	return dice;
}

bool ActionLibrary::hasAction(const std::string& name) {
		initialize();
		return actions.find(name) != actions.end();
}