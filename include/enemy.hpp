#pragma once
#include <iostream>
#include <box2d/box2d.h>
#include <string>
#include "collidable.hpp"
#include "player.hpp"
#include "types.hpp"
#include "render.hpp"

struct EnemyProperties {
    private:
    double default_health;
    public:
    double _health;
    double _damage;
    double _defense;
    double _agility;

    EnemyProperties(double health, double damage, double defense, double agility) 
        : _health(health), _damage(damage), _defense(defense), _agility(agility), default_health(health) {};
    EnemyProperties(const EnemyProperties &properties) 
        : EnemyProperties(properties._health, properties._damage, properties._defense, properties._agility) {};

    double get_default_health() const {return default_health;}
};

class Enemy;
class EnemyGUI {
private:
    const Enemy& enemy_;

    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarInner;

    void initHBar();
    
public:
    EnemyGUI(const Enemy& player);

    void updateHPBar();
    void renderHPBar(ZRenderer& renderer);

    ~EnemyGUI() = default;
};

class Enemy: public Collidable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|ENEMY|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|PLAYER|PROJECTILE|ENEMY;

    EnemyGUI _gui;

    void default_config();
public:
    b2Vec2 velocity;

    Enemy(float x, float y, b2World* world, Shapeb2* shape, std::string texture, EnemyProperties &&properties);
    Enemy(float x, float y, b2World* world, Shapeb2* shape, sf::Color texture, EnemyProperties &&properties); 
    
    void _move(sf::Vector2f& direction);
    void _move(Player& player);

    EnemyProperties& get_properties() {return _eProperties;}
    const EnemyProperties& get_properties() const {return _eProperties;}
    EnemyGUI& getGUI() {return _gui;}
    
protected:
    EnemyProperties _eProperties;
};

struct Enemies {
    std::vector<std::shared_ptr<Enemy>> enemies_;
    int spawn_delay;
    int counter;

    Enemies(int delay);

    void spawnEnemy(sf::RenderWindow& window, b2World& world, sf::View& camera, const Player& player);
    void handleEnemies(sf::RenderWindow& window);
};
