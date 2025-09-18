#ifndef DIE_HPP
# define DIE_HPP

# include <random>
#include <vector>
# include <iostream>

enum Action {
	ATTACK,
	HEAL,
	BLOCK,
	EMPTY
};

enum Target {
	ENEMY,
	ALLY,
	NONE
};

std::string actionToString(Action action);
std::string targetToString(Target target);

struct DiceFace {
		Action action;	// the action
		Target target;	// who it affects
		int value;					// How much damage/heal

		DiceFace(Action a, Target t, int v)
		: action(a), target(t), value(v) {}
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

		void displayFaces() const;
};

#endif