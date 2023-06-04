#pragma once
#include "GameUtilities.hpp"
#include "Weapon.hpp"

struct playerProperties {
    double _health;
    double _defense;
    double _agility;
    
    playerProperties(double health, double defense, double agility) 
        : _health(health), _defense(defense), _agility(agility) {}
    playerProperties(const playerProperties &properties) 
        : playerProperties(properties._health, properties._defense, properties._agility) {}
};

class Player: public Collidable {
private:
    static constexpr const u32 _categoryBits = PLAYER;
    static constexpr const u32 _maskBits = STATIC|PLAYER|ENEMY;

    b2Vec2 velocity;

public:
    Player(float x, float y, b2World* world, Shapeb2* shape, b2BodyType body_type, string texture, playerProperties &&properties, weapon weaponType);

    Weapon* get_weapon() {return _weapon;}


    void _move(RenderWindow &window);
    void _attack();

    ~Player();
protected:
    Weapon* _weapon;
    playerProperties _pProperties;
};
