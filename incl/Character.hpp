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
			std::string team;
			int max_health;
			int health;
			int incoming_damage;
			int shield;
			int round_death; // Track the round in which the character died
			Die die;

	public:
			Character(const std::string& name, int health, const std::vector<DiceFace>& customFaces, const std::string& team);
			virtual ~Character() {}	// Virtual destructor for proper inheritance

			// Character actions
			DiceFace roll();
			ActionResult takeDamage(int damage);
			ActionResult heal(int amount);
			ActionResult addShield(int amount);

			// Getters
			const std::string& getName() const;
			int getHealth() const;
			int getMaxHealth() const;
			int getIncomingDamage() const;
			int getShield() const;
			const std::string& getTeam() const;
			int getRoundOfDeath() const;

			// Setters
			void setRoundOfDeath(int round);
			int setIncomingDamage(int damage);
			void setMaxHealth(int newMaxHealth);
			void setHealth(int newHealth);
			void setShield(int newShield);

			// Utility
			void resetShield();
			bool isAlive() const;
			void displayDie() const;
			void displayTeamInfo(const Character& character);

};


#endif