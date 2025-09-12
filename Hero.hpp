// Hero.hpp
#ifndef HERO_HPP
# define HERO_HPP

# include "Character.hpp"

class Hero : public Character {
public:
    Hero(const std::string& hero_name, int hero_health)
    : Character(hero_name, hero_health, createHeroDie()) {}

private:
    std::vector<DiceFace> createHeroDie() {
        std::vector<DiceFace> faces;
        faces.push_back(DiceFace(DAMAGE, ENEMY, 3));
        faces.push_back(DiceFace(DAMAGE, ENEMY, 2));
				faces.push_back(DiceFace(SHIELD, SELF, 3));
				faces.push_back(DiceFace(SHIELD, SELF, 1));
				faces.push_back(DiceFace(HEAL, SELF, 2));
				faces.push_back(DiceFace(EMPTY, NONE, 0));
        return faces;
    }
};

#endif