#include "Die.hpp"

std::string actionToString(Action action) {
		switch(action) {
				case ATTACK: return "attack";
				case HEAL: return "heal";
				case BLOCK: return "block";
				case DODGE: return "dodge";
				case STUN: return "stun";
				case EMPTY: return "empty";
				default: return "unknown";
		}
}

Die::Die() : generator(std::random_device{}()), distribution(0, 5) {
}

Die::Die(const std::vector<DiceFace>& custom_faces): generator(std::random_device{}()), distribution(0, 5) {
		faces = custom_faces;
}

DiceFace Die::rollFace() {
		int roll = distribution(generator);
		return faces[roll];
}

const std::vector<DiceFace>& Die::getFaces() const {
		return faces;
}