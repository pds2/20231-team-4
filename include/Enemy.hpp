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
    static constexpr const u32 _categoryBits = ENEMY;
    static constexpr const u32 _maskBits = STATIC|PLAYER|PROJECTILE|ENEMY;

    b2Vec2 velocity;

public:
    Enemy(float x, float y, b2World* world, Shapeb2* shape, b2BodyType body_type, string texture, const enemyProperties &properties);
    void _move(Player& player);
    
protected:
    enemyProperties _eProperties;
};