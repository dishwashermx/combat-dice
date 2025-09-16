#include "Die.hpp"

std::string actionToString(Action action) {
		switch(action) {
				case ATTACK: return "attack";
				case HEAL: return "heal";
				case BLOCK: return "block";
				case EMPTY: return "empty";
				default: return "unknown";
		}
}

std::string targetToString(Target target) {
		switch(target) {
				case ENEMY: return "enemy";
				case ALLY: return "ally";
				case NONE: return "none";
				default: return "unknown";
		}
}

void Die::setupDefaultFaces() {
	faces.clear();
	faces.push_back(DiceFace(EMPTY, NONE, 0));
	faces.push_back(DiceFace(EMPTY, NONE, 2));
	faces.push_back(DiceFace(EMPTY, NONE, 1));
	faces.push_back(DiceFace(EMPTY, NONE, 2));
	faces.push_back(DiceFace(EMPTY, NONE, 1));
	faces.push_back(DiceFace(EMPTY, NONE, 0));
}

Die::Die() : generator(std::random_device{}()), distribution(0, 5) {
		setupDefaultFaces();
}

Die::Die(const std::vector<DiceFace>& custom_faces)
: generator(std::random_device{}()), distribution(0, 5) {
	if (custom_faces.size() == 6) {
		faces = custom_faces;}
	else {
		std::cout << "Custom die must have exactly 6 faces. Using default faces instead." << std::endl;
		setupDefaultFaces();
	}
}

DiceFace Die::rollFace() {
		int roll = distribution(generator);
		return faces[roll];
}

void Die::displayFaces() const {
	for (size_t i = 0; i < faces.size(); i++) {
			if (faces[i].action == ATTACK) {
					std::cout << "[ATTACK " << faces[i].value << "]";
			} else if (faces[i].action == HEAL) {
					std::cout << "[HEAL " << faces[i].value << "]";
			} else if (faces[i].action == BLOCK) {
					std::cout << "[BLOCK " << faces[i].value << "]";
			} else {
					std::cout << "[EMPTY]";
			}
			if (i < faces.size() - 1) {
					std::cout << " ";
			}
	}
	std::cout << std::endl;
}