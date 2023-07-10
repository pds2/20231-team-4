#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "collidable.hpp"
#include "weapon.hpp"
#include "assets.hpp"
#include "render.hpp"
#include "texttags.hpp"

struct PlayerProperties {
private:
    double default_health;

public:
    int level;
    double level_up;
    double _xp;
    double xpFieldRange;

    double _health;
    double _defense;
    double _agility;
    
    PlayerProperties(double health, double defense, double agility) 
        : _health(health), _defense(defense), _agility(agility), 
        _xp(0), level(0), level_up(50), xpFieldRange(10), default_health(health) {
            
    }
    PlayerProperties(PlayerProperties&& p) 
        : PlayerProperties(p._health, p._defense, p._agility) {
    }

    const double get_fieldRange() const {return xpFieldRange;}
    const double get_default_health() const {return default_health;}
    
    void update_xp(double xp);
};

struct PlayerGUI {
private:
    const Player& player_;

    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarInner;

    sf::RectangleShape xpBarBack;
    sf::RectangleShape xpBarInner;

    void initHPBar();
    void initXPBar();
    
public:
    PlayerGUI(const Player& player);

    void updateHPBar();
    void updateXPBar();
    void renderGUI(ZRenderer& renderer);

    ~PlayerGUI() = default;
};

struct XpField: public Collidable {
private:
    Player& player_;

    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|XPFIELD;
    static constexpr const u32 _maskBits = 0|XPORB;

public:
    XpField(Player& p); 

    void updateField();
    void handleField(TextTagSystem& tts);
    void upgradeField();

    ~XpField() = default;
};

class Player: public Collidable {
protected:
    Weapon* _weapon;
    PlayerProperties _pProperties;

private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|PLAYER|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|PLAYER|ENEMY;

    b2Vec2 velocity;
    PlayerGUI _gui;
    XpField xp_field;
    
    void default_config(WeaponType& weaponType);

public:
    Player(float x, float y, b2World* world, Shapeb2* shape, std::string texture, PlayerProperties&& properties, WeaponType weaponType);
    Player(float x, float y, b2World* world, Shapeb2* shape, sf::Color color, PlayerProperties&& properties, WeaponType weaponType);
    
    Weapon* get_weapon() {return _weapon;}
    PlayerProperties& get_properties() {return _pProperties;}
    const PlayerProperties& get_properties() const {return _pProperties;}
    PlayerGUI& getGUI() {return _gui;}
    const b2Vec2& get_velocity() const {return velocity;}

    void _move(sf::RenderWindow &window, sf::View& camera);
    void _attack(sf::RenderWindow &window);
    void handleAttack(sf::RenderWindow &window);

    void handlePlayer(TextTagSystem& tts);

    virtual ~Player();
};


class Frog: public Player {
private:
    static constexpr const double health = 100;
    static constexpr const double defense = 0.01;
    static constexpr const double agility = 2.5;
public:
    Frog(float x, float y, b2World* world, WeaponType weaponType);

    ~Frog() = default;
};