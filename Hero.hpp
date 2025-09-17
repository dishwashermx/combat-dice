// Hero.hpp
#ifndef HERO_HPP
# define HERO_HPP

# include "Character.hpp"

class Hero : public Character {
public:
    Hero(const std::string& hero_name, int hero_health, const std::string& team = "Hero")
    : Character(hero_name, hero_health, createHeroDie(), team) {}

private:
    std::vector<DiceFace> createHeroDie() {
        std::vector<DiceFace> faces;
        faces.push_back(DiceFace(ATTACK, ENEMY, 2));
        faces.push_back(DiceFace(ATTACK, ENEMY, 1));
        faces.push_back(DiceFace(ATTACK, ENEMY, 1));
				faces.push_back(DiceFace(ATTACK, ENEMY, 1));
				faces.push_back(DiceFace(BLOCK, ALLY, 1));
				faces.push_back(DiceFace(HEAL, ALLY, 1));
        return faces;
    }
};

#endif