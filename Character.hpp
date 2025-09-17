// Character.hpp
#ifndef CHARACTER_HPP
# define CHARACTER_HPP

# include <string>
# include <iostream>
# include "Die.hpp"

struct ActionResult {
    // Damage-specific
    int damageBlocked;
    int damageDealt;

    // Healing-specific
    int healingAttempted;
    int healingApplied;

    // Shield-specific
    int shieldAdded;

    // General state
    int newHealth;
    int newShield;
    int oldHealth;
    int oldShield;
};

class Character {
protected:
		std::string name;
		std::string team; // "Hero" or "Enemy"
		int max_health;
		int health;
		int incoming_damage;
		int shield;
		int round_death; // Track the round in which the character died
		Die die;

public:
		Character(const std::string& char_name, int char_health, const std::vector<DiceFace>& customFaces, const std::string& char_team = "")
		: name(char_name), team(char_team), max_health(char_health), health(char_health),
		  incoming_damage(0), shield(0), round_death(-1), die(customFaces) {}

		virtual ~Character() {}	// Virtual destructor for proper inheritance

		DiceFace roll() {
			DiceFace result = die.rollFace();
			return result;
		}

		ActionResult takeDamage(int damage) {
			int actualDamage = damage;
			ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield};

			if (shield > 0) {
				int blocked = std::min(shield, actualDamage);
				result.newShield -= blocked;
				shield -= blocked;
				actualDamage -= blocked;
				result.damageBlocked = blocked;
			}

			if (health == 0) {
				actualDamage = 0;
			}

			if (actualDamage > 0) {
				health -= actualDamage;
				if (health < 0)
					health = 0;
				result.damageDealt = actualDamage;
				result.newHealth = health;
			}
			return result;
		}

		ActionResult heal(int amount) {
			int appliedHealing = amount;
			ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield};

			if (health == max_health) {
				appliedHealing = 0; // No healing needed
			} else if (health + amount > max_health) {
				appliedHealing = max_health - health; // Heal to full health
				incoming_damage = 0; // Clear incoming damage when healed to full
			}

			health += appliedHealing;
			result.healingAttempted = amount;
			result.healingApplied = appliedHealing;
			result.newHealth = health;
			incoming_damage -= appliedHealing;
			if (incoming_damage < 0) incoming_damage = 0;

			return result;
		}

		ActionResult addShield(int amount) {
			ActionResult result = {0, 0, 0, 0, 0, health, shield, health, shield};

			shield += amount;
			result.shieldAdded = amount;
			result.newShield = shield;

			return result;
		}

		void resetShield() {
				shield = 0;
		}

		bool isAlive() const {
				return health > 0;
		}

		void setRoundOfDeath(int round) {
				round_death = round;
		}

		const std::string& getName() const {
				return name;
		}

		int getHealth() const {
				return health;
		}

		int getMaxHealth() const {
				return max_health;
		}

		int getIncomingDamage() const {
				return incoming_damage;
		}

		int setIncomingDamage(int damage) {
				incoming_damage = damage;
				return incoming_damage;
		}

		int getShield() const {
				return shield;
		}

		const std::string& getTeam() const {
				return team;
		}

		void displayDie() const {
  		die.displayFaces();
		}

		void displayTeamInfo(const Character& character) {
				std::cout << character.getName() << " is on team: " << character.getTeam() << std::endl;
		}

		int getRoundOfDeath() const {
				return round_death;
		}
};


#endif