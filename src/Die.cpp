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

void Die::setupDefaultFaces() {
	faces.clear();
	faces.push_back(DiceFace(EMPTY, 0));
	faces.push_back(DiceFace(EMPTY, 2));
	faces.push_back(DiceFace(EMPTY, 1));
	faces.push_back(DiceFace(EMPTY, 2));
	faces.push_back(DiceFace(EMPTY, 1));
	faces.push_back(DiceFace(EMPTY, 0));
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

const std::vector<DiceFace>& Die::getFaces() const {
		return faces;
}