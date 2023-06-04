#pragma once
#include "GameUtilities.hpp"

struct enemyProperties {
    double _health;
    double _damage;
    double _defense;
    double _agility;

    enemyProperties(double health, double damage, double defense, double agility) 
        : _health(health), _damage(damage), _defense(defense), _agility(agility) {};
    enemyProperties(const enemyProperties &properties) 
        : enemyProperties(properties._health, properties._damage, properties._defense, properties._agility) {};
};

class Enemy: public Collidable {
private:
    b2Vec2 velocity;
public:
    Enemy(float x, float y, Shapeb2* shape, b2BodyType body_type, string texture, b2World* world, const enemyProperties &properties);
    void _move(Player& player);
protected:
    enemyProperties _eProperties;
};