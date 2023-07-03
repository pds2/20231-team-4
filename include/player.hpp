#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "collidable.hpp"
#include "weapon.hpp"
#include "assets.hpp"

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

    std::vector<sf::Drawable*> get_gui_drawables();

    void updateHPBar();

    ~PlayerGUI() = default;
};

class Player: public Collidable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|PLAYER|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|PLAYER|ENEMY;

    PlayerGUI _gui;

    void default_config(WeaponType& weaponType);
    
public:
    b2Vec2 velocity;

    Player(float x, float y, b2World* world, Shapeb2* shape, std::string texture, PlayerProperties &&pProperties, WeaponType weaponType);
    Player(float x, float y, b2World* world, Shapeb2* shape, sf::Color color, PlayerProperties &&pProperties, WeaponType weaponType);
    
    Weapon* get_weapon() {return _weapon;}
    PlayerProperties& get_properties() {return _pProperties;}
    const PlayerProperties& get_properties() const {return _pProperties;}
    std::vector<sf::Drawable*> get_drawables() {return _drawables;}

    void _move(sf::RenderWindow &window, sf::View& camera);
    void _attack();
    
    ~Player();
protected:
    Weapon* _weapon;
    PlayerProperties _pProperties;
    std::vector<sf::Drawable*> _drawables;
};
