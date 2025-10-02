#include "ActionLibrary.hpp"
#include <iostream>
#include <iomanip>

std::unordered_map<std::string, DiceFace> ActionLibrary::actions;
bool ActionLibrary::initialized = false;

void ActionLibrary::initialize() {
	if (initialized)
		return;

	// === ATTACK ACTIONS ===
	actions["unarmed-strike"] = DiceFace("Unarmed Strike", "punches", "🤜", ATTACK, 1);
	actions["claw-attack"] = DiceFace("Claw Attack", "scratches", "␥", ATTACK, 2);
	actions["sword-slash"] = DiceFace("Sword Slash", "slashes", "🗡️", ATTACK, 2);
	actions["dagger"] = DiceFace("Dagger", "stabs", "🗡️", ATTACK, 1);
	actions["backstab"] = DiceFace("Backstab", "backstabs", "🗡️", ATTACK, 4);

	// === BLOCK ACTIONS ===
	actions["raise-arms"] = DiceFace("Raise Arms", "blocks", "🫸", BLOCK, 1);
	actions["buckler"] = DiceFace("Buckler", "shields", "🛡️", BLOCK, 2);

	// === HEAL ACTIONS ===
	actions["small-vial"] = DiceFace("Small Vial", "heals", "💖", HEAL, 1);

	// === SPECIAL ACTIONS ===
	actions["dodge"] = DiceFace("Dodge", "dodges", "💨", DODGE, 0);
	actions["stun"] = DiceFace("Stun", "stuns", "💫", STUN, 0);
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