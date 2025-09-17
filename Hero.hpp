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
        faces.push_back(DiceFace(ATTACK, ENEMY, 3));
        faces.push_back(DiceFace(ATTACK, ENEMY, 3));
        faces.push_back(DiceFace(ATTACK, ENEMY, 1));
				faces.push_back(DiceFace(BLOCK, ALLY, 2));
				faces.push_back(DiceFace(BLOCK, ALLY, 2));
				faces.push_back(DiceFace(HEAL, ALLY, 2));
        return faces;
    }
};

#endif