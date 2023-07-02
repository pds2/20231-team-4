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

    void allocateWeapon(WeaponType& weaponType);
public:
    b2Vec2 velocity;

    Player(float x, float y, b2World* world, Shapeb2* shape, std::string texture, PlayerProperties &&pProperties, WeaponType weaponType);
    Player(float x, float y, b2World* world, Shapeb2* shape, sf::Color color, PlayerProperties &&pProperties, WeaponType weaponType);
    
    Weapon* get_weapon() {return _weapon;}

    void _move(sf::RenderWindow &window, sf::View& camera);
    void _attack();

    PlayerProperties& get_properties() {return _pProperties;}

    void operator=(Player&& player);

    ~Player();
protected:
    Weapon* _weapon;
    PlayerProperties _pProperties;
};
