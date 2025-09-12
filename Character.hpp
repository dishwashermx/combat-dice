// Character.hpp
#ifndef CHARACTER_HPP
# define CHARACTER_HPP

# include <string>
# include <iostream>
# include "Die.hpp"

class Character {
protected:
		std::string name;
		int max_health;
		int health;
		int shield;
		Die die;

public:
		Character(const std::string& char_name, int char_health)
		: name(char_name), max_health(char_health), health(char_health), shield(0), die() {}

		Character(const std::string& char_name, int char_health, const std::vector<DiceFace>& customFaces)
		: name(char_name), max_health(char_health), health(char_health), shield(0), die(customFaces) {}

		virtual ~Character() {}	// Virtual destructor for proper inheritance

		DiceFace roll() {
			DiceFace result = die.rollFace();
			std::cout << name << " rolls a " << result.value << " (" << actionToString(result.action) << ")!" << std::endl;
			return result;
		}

		void takeDamage(int damage) {
			int actualDamage = damage;
				if (shield > 0) {
						int blocked = std::min(shield, actualDamage);
						shield = shield - blocked;
						actualDamage = actualDamage - blocked;
						std::cout << blocked << " damage was blocked. Shield now " << shield << std::endl;
					}
					if (actualDamage > 0) {
						health = health - actualDamage;
						if (health < 0)
						health = 0;
						std::cout << name << " takes " << actualDamage << " damage, health now " << health << std::endl;
				}
		}

		void heal(int amount) {
				if (health + amount > max_health)
						amount = max_health - health;
				health += amount;
				std::cout << name << " heals " << amount << ", health now " << health << std::endl;
		}

		void addShield(int amount) {
				shield += amount;
				std::cout << name << " gains " << amount << " shield, total shield now " << shield << std::endl;
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

		void displayDie() const {
  		die.displayFaces();
		}
};

#endif