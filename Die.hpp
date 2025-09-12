#ifndef DIE_HPP
# define DIE_HPP

# include <random>
#include <vector>
# include <iostream>

enum Action {
	DAMAGE,
	HEAL,
	SHIELD,
	EMPTY
};

enum Target {
	ENEMY,
	SELF,
	NONE
};

std::string actionToString(Action action) {
		switch(action) {
				case DAMAGE: return "damage";
				case HEAL: return "heal";
				case SHIELD: return "shield";
				case EMPTY: return "empty";
				default: return "unknown";
		}
}

std::string targetToString(Target target) {
		switch(target) {
				case ENEMY: return "enemy";
				case SELF: return "self";
				case NONE: return "none";
				default: return "unknown";
		}
}

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

		void setupDefaultFaces() {
			faces.clear();
			faces.push_back(DiceFace(EMPTY, NONE, 0));
			faces.push_back(DiceFace(EMPTY, NONE, 2));
			faces.push_back(DiceFace(EMPTY, NONE, 1));
			faces.push_back(DiceFace(EMPTY, NONE, 2));
			faces.push_back(DiceFace(EMPTY, NONE, 1));
			faces.push_back(DiceFace(EMPTY, NONE, 0));
    }

public:
		Die() : generator(std::random_device{}()), distribution(0, 5) {
				setupDefaultFaces();
		}

		Die(const std::vector<DiceFace>& custom_faces)
		: generator(std::random_device{}()), distribution(0, 5) {
			if (custom_faces.size() == 6) {
				faces = custom_faces;}
			else {
				std::cout << "Custom die must have exactly 6 faces. Using default faces instead." << std::endl;
				setupDefaultFaces();
			}
		}

		DiceFace rollFace() {
				int roll = distribution(generator);
				return faces[roll];
		}

		void displayFaces() const {
			std::cout << "Die faces: ";
			for (size_t i = 0; i < faces.size(); i++) {
					if (faces[i].action == DAMAGE) {
							std::cout << "[DAMAGE " << faces[i].value << "]";
					} else if (faces[i].action == HEAL) {
							std::cout << "[HEAL " << faces[i].value << "]";
					} else if (faces[i].action == SHIELD) {
							std::cout << "[SHIELD " << faces[i].value << "]";
					} else {
							std::cout << "[EMPTY]";
					}
					if (i < faces.size() - 1) {
							std::cout << " ";
					}
			}
			std::cout << std::endl;
	}
};

#endif