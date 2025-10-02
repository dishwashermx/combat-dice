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

		// Default constructor for containers
		DiceFace() : name(""), desc(""), emoji(""), action(EMPTY), value(0), marks(NO_MARK) {}

		// Full constructor with validation
		DiceFace(std::string n, std::string d, std::string e, Action a, int v, int m = NO_MARK)
		: name(n), desc(d), emoji(e), action(a), value(v), marks(m) {}
};

class Die {
private:
		std::mt19937 generator;
		std::uniform_int_distribution<int> distribution;
		std::vector<DiceFace> faces;

public:
		Die();
		Die(const std::vector<DiceFace>& custom_faces);

		DiceFace rollFace();
		const std::vector<DiceFace>& getFaces() const;
};

std::string actionToString(Action action);

#endif