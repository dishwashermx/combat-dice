#ifndef DIE_HPP
# define DIE_HPP

# include <random>
#include <vector>
# include <iostream>

enum Action {
	ATTACK,
	HEAL,
	BLOCK,
	DODGE,
	STUN,
	EMPTY
};

enum Mark {
	NO_MARK = 0,
	SPLASH = 1 << 0,     // Adjacent targets
	WAVE = 1 << 1,     // Target team
	QUAKE = 1 << 2       // Everyone alive
};

struct DiceFace {
		std::string name;  // name of the action
		std::string desc;  // description of the action
		std::string emoji; // emoji representation
		Action action;     // base action
		int value;         // how much
		int marks;         // special properties (bitmask of Mark enum)

		// Full constructor with validation
		DiceFace(std::string n, std::string d, std::string e, Action a, int v, int m = NO_MARK)
		: name(n), desc(d), emoji(e), action(a), value(v), marks(m) {
			// Runtime validation
			if (value < 0) {
				std::cerr << "Warning: DiceFace '" << name << "' has negative value: " << value << std::endl;
			}
			if (name.empty() && !desc.empty()) {
				std::cerr << "Warning: DiceFace has description but no name" << std::endl;
			}
		}

		// Simple constructor for legacy compatibility with validation
		DiceFace(Action a, int v, int m = NO_MARK)
		: name(""), desc(""), emoji(""), action(a), value(v), marks(m) {
			// Runtime validation
			if (v < 0) {
				std::cerr << "Warning: DiceFace has negative value: " << v << std::endl;
			}
		}

		// Deleted constructors to prevent common mistakes
		DiceFace(Action a, int v, int m, int extra) = delete;  // Prevent 4-param calls
		DiceFace(std::string n, Action a, int v) = delete;     // Prevent missing params

		// Factory functions for safer construction
		static DiceFace createAttack(const std::string& name, int damage, int marks = NO_MARK) {
			return DiceFace(name, "attacks", "⚔️", ATTACK, damage, marks);
		}

		static DiceFace createBlock(const std::string& name, int shield, int marks = NO_MARK) {
			return DiceFace(name, "blocks", "🛡️", BLOCK, shield, marks);
		}

		static DiceFace createHeal(const std::string& name, int healing, int marks = NO_MARK) {
			return DiceFace(name, "heals", "💖", HEAL, healing, marks);
		}

		static DiceFace createEmpty() {
			return DiceFace("Empty", "No action", "❌", EMPTY, 0, NO_MARK);
		}
};

class Die {
private:
		std::mt19937 generator;
		std::uniform_int_distribution<int> distribution;
		std::vector<DiceFace> faces;

		void setupDefaultFaces();

public:
		Die();
		Die(const std::vector<DiceFace>& custom_faces);

		DiceFace rollFace();
		const std::vector<DiceFace>& getFaces() const;
};

std::string actionToString(Action action);

#endif