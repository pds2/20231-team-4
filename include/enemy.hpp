#pragma once
#include <iostream>
#include <box2d/box2d.h>
#include <string>

#include "collidable.hpp"
#include "player.hpp"
#include "types.hpp"
#include "render.hpp"
#include "tilemap.hpp"

struct EnemyProperties {
private:
    std::pair<int, int> xp_range;
    double default_health;

public:
    int level;
    double _health;
    double _defense;
    double _agility;

    double _xp;

    double _damage;
    int damage_delay;

    EnemyProperties(double health, double damage, double damage_delay, double defense, double agility, std::pair<int, int> xpRange) 
        : _health(health), _damage(damage), damage_delay(damage_delay), 
        _defense (defense), _agility(agility), default_health(health), 
        xp_range(xpRange), level(1) {
        
        _xp = xp_range.first+(std::rand()%xp_range.second);
    };
    EnemyProperties(EnemyProperties&& properties) 
        : EnemyProperties(properties._health, properties._damage, properties.damage_delay, properties._defense, properties._agility, properties.getXpRange()) {};

    double get_default_health() const {return default_health;}
    std::pair<int, int> getXpRange() const {return xp_range;}

    void setXpRange(std::pair<int, int> new_range) {
        xp_range = new_range;
        _xp = xp_range.first+(std::rand()%xp_range.second);
    }
};

class Enemy;
struct EnemyGUI {
private:
    const Enemy& enemy_;

    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarInner;

    void initHBar();
    
public:
    EnemyGUI(const Enemy& player);

    void updateHPBar();
    void renderGUI(ZRenderer& renderer);

    ~EnemyGUI() = default;
};

class Enemy;
class XpOrb: public Collidable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|XPORB;
    static constexpr const u32 _maskBits = 0|XPFIELD;

    double xp;
public:
    XpOrb(Enemy& e);

    void renderOrb(ZRenderer& renderer);

    ~XpOrb() = default;
};


class Enemy: public Collidable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|ENEMY|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|PLAYER|PROJECTILE|ENEMY;

    b2Vec2 velocity;
    EnemyGUI _gui;
    
    void default_config();
public:

    Enemy(float x, float y, b2World* world, Shapeb2* shape, std::string texture, EnemyProperties &&properties);
    Enemy(float x, float y, b2World* world, Shapeb2* shape, sf::Color color, EnemyProperties &&properties); 
    
    void _move(sf::Vector2f& direction, sf::RenderWindow& window);
    void _move(Player& player, sf::RenderWindow& window);

    EnemyProperties& get_properties() {return _eProperties;}
    const EnemyProperties& get_properties() const {return _eProperties;}
    const b2Vec2& get_velocity() const {return velocity;}
    EnemyGUI& getGUI() {return _gui;}
    
protected:
    EnemyProperties _eProperties;
};

struct Enemies {
    std::vector<std::shared_ptr<Enemy>> enemies_;
    int spawn_delay;
    int counter;

    std::vector<std::shared_ptr<XpOrb>> xpOrbs_;

    Enemies(int delay);

    void spawnEnemy(sf::RenderWindow& window, b2World& world, sf::View& camera, const Player& player);
    void handleEnemies();
    void handleOrbs();
};

class Bugol: public Enemy {
private:
    static constexpr const std::pair<int, int> xp_range = {1, 3};
    static constexpr const double health = 30;
    static constexpr const double defense = 0;
    static constexpr const double agility = 2;
    static constexpr const int damage_delay = 100;
    static constexpr const double damage = 10;

public:
    Bugol(float x, float y, b2World* world);
    
};