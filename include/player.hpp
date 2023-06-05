#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "collidable.hpp"
#include "weapon.hpp"

struct PlayerProperties {
    double _health;
    double _defense;
    double _agility;
    
    PlayerProperties(double health, double defense, double agility) 
        : _health(health), _defense(defense), _agility(agility) {}
    PlayerProperties(const PlayerProperties &properties) 
        : PlayerProperties(properties._health, properties._defense, properties._agility) {}
};

class Player: public Collidable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|PLAYER|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|PLAYER|ENEMY;

    b2Vec2 velocity;

public:
    Player(float x, float y, b2World* world, Shapeb2* shape, b2BodyType body_type, std::string texture, PlayerProperties &&properties, WeaponType weaponType);

    Weapon* get_weapon() {return _weapon;}


    void _move(sf::RenderWindow &window);
    void _attack();

    ~Player();
protected:
    Weapon* _weapon;
    PlayerProperties _pProperties;
};
