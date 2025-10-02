#ifndef ACTION_LIBRARY_HPP
#define ACTION_LIBRARY_HPP

#include "Die.hpp
#include <vector>
#include <string>
#include <unordered_map>


struct ActionTemplate {
    std::string name;
    std::string description;
    DiceFace face;
    std::string emoji;

    ActionTemplate(const std::string& n, const std::string& desc,
                  Action action, Target target, int value, const std::string& e = "")
        : name(n), description(desc), face(action, target, value), emoji(e) {}
};

/**
 * ActionLibrary - Central registry of all available actions
 *
 * Usage:
 *   auto fireball = ActionLibrary::getDiceFace("fireball");
 *   auto knightDice = ActionLibrary::createDiceSet({"sword_slash", "shield_up", "cleave"});
 */
class ActionLibrary {
private:
    static std::unordered_map<std::string, ActionTemplate> actions;
    static bool initialized;
    static void initialize();

public:
    // Get a specific action by name
    static ActionTemplate getAction(const std::string& name);

    // Get the DiceFace for an action
    static DiceFace getDiceFace(const std::string& name);

    // Check if action exists
    static bool hasAction(const std::string& name);

    // Get all available action names
    static std::vector<std::string> getAllActionNames();

    // Create a dice set from action names
    static std::vector<DiceFace> createDiceSet(const std::vector<std::string>& actionNames);

    // Utility functions
    static void printAction(const std::string& name);
    static void printAllActions();
};

/**
 * CharacterBuilds - Predefined character archetypes
 *
 * Usage:
 *   auto knightDice = ActionLibrary::createDiceSet(CharacterBuilds::WARRIOR());
 */
class CharacterBuilds {
public:
    static std::vector<std::string> WARRIOR() {
        return {"sword_slash", "heavy_strike", "cleave", "shield_up", "armor_buff", "battle_cry"};
    }

    static std::vector<std::string> MAGE() {
        return {"fireball", "lightning_bolt", "ice_storm", "healing_wave", "mana_shield", "focus"};
    }

    static std::vector<std::string> PALADIN() {
        return {"holy_strike", "lay_on_hands", "divine_protection", "smite", "blessing", "sanctuary"};
    }

    static std::vector<std::string> ASSASSIN() {
        return {"backstab", "poison_blade", "shadow_strike", "vampiric_drain", "stealth", "preparation"};
    }

    static std::vector<std::string> BERSERKER() {
        return {"rage_attack", "whirlwind", "brutal_swing", "blood_frenzy", "reckless_charge", "exhaustion"};
    }

    static std::vector<std::string> CLERIC() {
        return {"healing_light", "group_heal", "sanctuary", "turn_undead", "blessing", "divine_favor"};
    }

    static std::vector<std::string> RANGER() {
        return {"precise_shot", "multi_shot", "hunter_mark", "nature_heal", "evasion", "track"};
    }
};

#endif