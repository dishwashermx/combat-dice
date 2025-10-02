#ifndef ACTION_LIBRARY_HPP
#define ACTION_LIBRARY_HPP

#include "Die.hpp"
#include <vector>
#include <string>
#include <unordered_map>

class ActionLibrary {
	private:
		static std::unordered_map<std::string, DiceFace> actions;
		static bool initialized;
		static void initialize();

	public:
		static bool hasAction(const std::string& name);
		static DiceFace getAction(const std::string& name);
		static std::vector<DiceFace> createDiceSet(const std::vector<std::string>& actionNames);
} ;

#endif