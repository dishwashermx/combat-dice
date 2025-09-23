#include "ActionLibrary.hpp"
#include <iostream>
#include <iomanip>

// Static member definitions
std::unordered_map<std::string, ActionTemplate> ActionLibrary::actions;
bool ActionLibrary::initialized = false;

void ActionLibrary::initialize() {
    if (initialized) return;

    // === ATTACK ACTIONS ===
    actions["sword_slash"] = ActionTemplate("Sword Slash", "A basic melee attack", ATTACK, MONSTER, 3, "⚔️");
    actions["heavy_strike"] = ActionTemplate("Heavy Strike", "A powerful single attack", ATTACK, MONSTER, 4, "💥");
    actions["cleave"] = ActionTemplate("Cleave", "Attack adjacent enemies", ATTACK, ADJ_MONSTERS, 2, "🌊");
    actions["whirlwind"] = ActionTemplate("Whirlwind", "Spin attack hitting nearby foes", ATTACK, ADJ_MONSTERS, 2, "🌪️");

    // === MAGICAL ATTACKS ===
    actions["fireball"] = ActionTemplate("Fireball", "Explosive magic damaging all enemies", ATTACK, ALL_MONSTERS, 3, "🔥");
    actions["lightning_bolt"] = ActionTemplate("Lightning Bolt", "Chain lightning through enemies", ATTACK, ALL_MONSTERS, 2, "⚡");
    actions["ice_storm"] = ActionTemplate("Ice Storm", "Freezing area damage", ATTACK, ALL_MONSTERS, 2, "❄️");
    actions["holy_strike"] = ActionTemplate("Holy Strike", "Righteous divine attack", ATTACK, MONSTER, 3, "✨");
    actions["smite"] = ActionTemplate("Smite", "Powerful divine punishment", ATTACK, MONSTER, 4, "🌟");

    // === SPECIAL ATTACKS ===
    actions["backstab"] = ActionTemplate("Backstab", "High damage sneak attack", ATTACK, MONSTER, 5, "🗡️");
    actions["poison_blade"] = ActionTemplate("Poison Blade", "Toxic strike", ATTACK, MONSTER, 2, "☠️");
    actions["shadow_strike"] = ActionTemplate("Shadow Strike", "Attack from the shadows", ATTACK, MONSTER, 3, "🌑");
    actions["vampiric_drain"] = ActionTemplate("Vampiric Drain", "Attack that heals self", ATTACK, MONSTER, 2, "🩸");
    actions["rage_attack"] = ActionTemplate("Rage Attack", "Berserker fury strike", ATTACK, MONSTER, 4, "😡");

    // === HEALING ACTIONS ===
    actions["healing_light"] = ActionTemplate("Healing Light", "Basic healing spell", HEAL, HERO, 3, "💚");
    actions["lay_on_hands"] = ActionTemplate("Lay on Hands", "Divine healing touch", HEAL, HERO, 3, "🙏");
    actions["healing_wave"] = ActionTemplate("Healing Wave", "Area healing magic", HEAL, ALL_HEROES, 2, "🌊");
    actions["group_heal"] = ActionTemplate("Group Heal", "Heal all party members", HEAL, ALL_HEROES, 2, "💚");

    // === DEFENSIVE ACTIONS ===
    actions["shield_up"] = ActionTemplate("Shield Up", "Raise defensive shield", BLOCK, HERO, 3, "🛡️");
    actions["armor_buff"] = ActionTemplate("Armor Buff", "Strengthen nearby allies", BLOCK, ADJ_HEROES, 2, "⚡");
    actions["divine_protection"] = ActionTemplate("Divine Protection", "Holy shield for all", BLOCK, ALL_HEROES, 1, "✨");
    actions["mana_shield"] = ActionTemplate("Mana Shield", "Magical barrier", BLOCK, HERO, 2, "🔮");
    actions["sanctuary"] = ActionTemplate("Sanctuary", "Sacred protection", BLOCK, ALL_HEROES, 2, "🏛️");

    // === UTILITY ACTIONS ===
    actions["focus"] = ActionTemplate("Focus", "Concentrate power", EMPTY, NONE, 0, "🧘");
    actions["stealth"] = ActionTemplate("Stealth", "Hide in shadows", EMPTY, NONE, 0, "👤");
    actions["preparation"] = ActionTemplate("Preparation", "Prepare for next action", EMPTY, NONE, 0, "⏳");
    actions["battle_cry"] = ActionTemplate("Battle Cry", "Inspire allies", EMPTY, NONE, 0, "📢");

    initialized = true;
}

ActionTemplate ActionLibrary::getAction(const std::string& name) {
    initialize();
    auto it = actions.find(name);
    if (it != actions.end()) {
        return it->second;
    }
    // Return a default "nothing" action if not found
    return ActionTemplate("Unknown", "Unknown action", EMPTY, NONE, 0, "❓");
}

DiceFace ActionLibrary::getDiceFace(const std::string& name) {
    return getAction(name).face;
}

bool ActionLibrary::hasAction(const std::string& name) {
    initialize();
    return actions.find(name) != actions.end();
}

std::vector<std::string> ActionLibrary::getAllActionNames() {
    initialize();
    std::vector<std::string> names;
    for (const auto& pair : actions) {
        names.push_back(pair.first);
    }
    return names;
}

std::vector<DiceFace> ActionLibrary::createDiceSet(const std::vector<std::string>& actionNames) {
    std::vector<DiceFace> dice;
    for (const std::string& name : actionNames) {
        dice.push_back(getDiceFace(name));
    }
    return dice;
}

void ActionLibrary::printAction(const std::string& name) {
    ActionTemplate action = getAction(name);
    std::cout << action.emoji << " " << action.name << std::endl;
    std::cout << "   " << action.description << std::endl;
    std::cout << "   Action: " << actionToString(action.face.action)
              << " | Target: " << targetToString(action.face.target)
              << " | Value: " << action.face.value << std::endl;
}

void ActionLibrary::printAllActions() {
    initialize();
    std::cout << "\n=== ACTION LIBRARY ===" << std::endl;

    std::cout << "\n--- ATTACK ACTIONS ---" << std::endl;
    for (const auto& pair : actions) {
        if (pair.second.face.action == ATTACK) {
            std::cout << std::setw(15) << pair.first << " | " << pair.second.emoji
                      << " " << pair.second.name << std::endl;
        }
    }

    std::cout << "\n--- HEALING ACTIONS ---" << std::endl;
    for (const auto& pair : actions) {
        if (pair.second.face.action == HEAL) {
            std::cout << std::setw(15) << pair.first << " | " << pair.second.emoji
                      << " " << pair.second.name << std::endl;
        }
    }

    std::cout << "\n--- DEFENSIVE ACTIONS ---" << std::endl;
    for (const auto& pair : actions) {
        if (pair.second.face.action == BLOCK) {
            std::cout << std::setw(15) << pair.first << " | " << pair.second.emoji
                      << " " << pair.second.name << std::endl;
        }
    }

    std::cout << "\n--- UTILITY ACTIONS ---" << std::endl;
    for (const auto& pair : actions) {
        if (pair.second.face.action == EMPTY) {
            std::cout << std::setw(15) << pair.first << " | " << pair.second.emoji
                      << " " << pair.second.name << std::endl;
        }
    }
}