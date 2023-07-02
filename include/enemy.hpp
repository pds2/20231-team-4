#pragma once
#include <iostream>
#include <box2d/box2d.h>
#include <string>
#include "collidable.hpp"
#include "player.hpp"
#include "types.hpp"

struct EnemyProperties {
    double _health;
    double _damage;
    double _defense;
    double _agility;

    EnemyProperties(double health, double damage, double defense, double agility) 
        : _health(health), _damage(damage), _defense(defense), _agility(agility) {};
    EnemyProperties(const EnemyProperties &properties) 
        : EnemyProperties(properties._health, properties._damage, properties._defense, properties._agility) {};
};

class Enemy: public Collidable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|ENEMY|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|PLAYER|PROJECTILE|ENEMY;

    void default_config();
public:
    Enemy(float x, float y, b2World* world, Shapeb2* shape, std::string texture, EnemyProperties &&properties);
    Enemy(float x, float y, b2World* world, Shapeb2* shape, sf::Color texture, EnemyProperties &&properties); 
    
    void _move(sf::Vector2f& direction);
    void _move(Player& player);

    EnemyProperties& get_properties() {return _eProperties;}
    
protected:
    EnemyProperties _eProperties;
};

struct Enemies {
    std::vector<std::shared_ptr<Enemy>> enemies_;
    int spawn_delay {0};
};