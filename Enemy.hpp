// Enemy.hpp
#ifndef ENEMY_HPP
# define ENEMY_HPP

# include "Character.hpp"

class Enemy : public Character {
public:
    Enemy(const std::string& enemy_name, int enemy_health)
    : Character(enemy_name, enemy_health, createEnemyDie()) {}

private:
    std::vector<DiceFace> createEnemyDie() {
        std::vector<DiceFace> faces;
        faces.push_back(DiceFace(DAMAGE, ENEMY, 3));
        faces.push_back(DiceFace(DAMAGE, ENEMY, 2));
        faces.push_back(DiceFace(DAMAGE, ENEMY, 1));
        faces.push_back(DiceFace(DAMAGE, ENEMY, 1));
        faces.push_back(DiceFace(EMPTY, ENEMY, 1));
        faces.push_back(DiceFace(EMPTY, ENEMY, 0));
        return faces;
    }
};

#endif