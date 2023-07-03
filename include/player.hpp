#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "collidable.hpp"
#include "weapon.hpp"
#include "assets.hpp"
#include "render.hpp"

struct PlayerProperties {
    double _health;
    double _defense;
    double _agility;
    
    PlayerProperties(double health, double defense, double agility) 
        : _health(health), _defense(defense), _agility(agility) {}
    PlayerProperties(const PlayerProperties &properties) 
        : PlayerProperties(properties._health, properties._defense, properties._agility) {}
};

class PlayerGUI {
private:
    const Player& player_;

    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarInner;

    void initHBar();
    
public:
    PlayerGUI(const Player& player);

    void updateHPBar();
    void renderHPBar(ZRenderer& renderer);

    ~PlayerGUI() = default;
};

class Player: public Collidable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|PLAYER|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|PLAYER|ENEMY;

    b2Vec2 velocity;
    PlayerGUI _gui;

    void default_config(WeaponType& weaponType);
    
public:

    Player(float x, float y, b2World* world, Shapeb2* shape, std::string texture, PlayerProperties &&pProperties, WeaponType weaponType);
    Player(float x, float y, b2World* world, Shapeb2* shape, sf::Color color, PlayerProperties &&pProperties, WeaponType weaponType);
    
    Weapon* get_weapon() {return _weapon;}
    PlayerProperties& get_properties() {return _pProperties;}
    const PlayerProperties& get_properties() const {return _pProperties;}
    const b2Vec2& get_velocity() const {return velocity;}

    void _move(sf::RenderWindow &window, sf::View& camera);
    void _attack(sf::RenderWindow &window);
    void handleAttack(sf::RenderWindow &window);
    PlayerGUI& getGUI() {return _gui;}

    ~Player();
protected:
    Weapon* _weapon;
    PlayerProperties _pProperties;
};
