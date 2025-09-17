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
		int max_health;
		int health;
		int incoming_damage;
		int shield;
		Die die;

public:
		Character(const std::string& char_name, int char_health)
		: name(char_name), max_health(char_health), health(char_health), incoming_damage(0), shield(0), die() {}

		Character(const std::string& char_name, int char_health, const std::vector<DiceFace>& customFaces)
		: name(char_name), max_health(char_health), health(char_health), incoming_damage(0), shield(0), die(customFaces) {}

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
				actualDamage -= blocked;
				result.damageBlocked = blocked;
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
			}

			health += appliedHealing;
			result.healingAttempted = amount;
			result.healingApplied = appliedHealing;
			result.newHealth = health;

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

		void displayDie() const {
  		die.displayFaces();
		}
};

#endif