// Enemy.hpp
#ifndef ENEMY_HPP
# define ENEMY_HPP

# include "Character.hpp"

class Enemy : public Character {
public:
    Enemy(const std::string& enemy_name, int enemy_health, const std::string& team = "Enemy")
    : Character(enemy_name, enemy_health, createEnemyDie(), team) {}

private:
    std::vector<DiceFace> createEnemyDie() {
        std::vector<DiceFace> faces;
        faces.push_back(DiceFace(ATTACK, ENEMY, 3));
        faces.push_back(DiceFace(ATTACK, ENEMY, 2));
        faces.push_back(DiceFace(ATTACK, ENEMY, 1));
        faces.push_back(DiceFace(ATTACK, ENEMY, 1));
        faces.push_back(DiceFace(ATTACK, ENEMY, 1));
        faces.push_back(DiceFace(EMPTY, ENEMY, 1));
        return faces;
    }
};


#endif