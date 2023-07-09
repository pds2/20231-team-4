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
    int level;

    std::pair<double, double> xp_range;
    double _health;
    double _agility;
    double _damage;
    int damage_delay;

    EnemyProperties(double health, double damage, double damage_delay, double agility, std::pair<int, int> xpRange) 
        : _health(health), _damage(damage), damage_delay(damage_delay), 
        _agility(agility), 
        xp_range(xpRange), level(0) {
    };
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
    
public:
    int level;
    double _health;
    double _xp;

    Enemy(float x, float y, b2World* world, Shapeb2* shape, std::string texture);
    Enemy(float x, float y, b2World* world, Shapeb2* shape, sf::Color color); 
    
    virtual EnemyProperties& get_properties() = 0;
    virtual const EnemyProperties& get_properties() const = 0;
    virtual void level_up() = 0;

    void _move(sf::Vector2f& direction, sf::RenderWindow& window);
    void _move(Player& player, sf::RenderWindow& window);

    const b2Vec2& get_velocity() const {return velocity;}
    EnemyGUI& getGUI() {return _gui;}


   virtual ~Enemy() = default;
};


struct Enemies {
    std::vector<std::shared_ptr<Enemy>> enemies_;
    int spawn_delay;
    int counter;

    std::vector<std::shared_ptr<XpOrb>> xpOrbs_;

    Enemies(int delay);

    void spawnEnemy(sf::RenderWindow& window, b2World& world, sf::View& camera, const Player& player);
    void handleEnemies(TextTagSystem& tts);
    void handleOrbs();
};


class Bugol: public Enemy {
private:
    static constexpr const std::pair<double, double> xp_range = {1, 2};
    static constexpr const double health = 5;
    static constexpr const double agility = 1;
    static constexpr const int damage_delay = 100;
    static constexpr const double damage = 1;

    static constexpr const double xp_range_buff = 0.5;
    static constexpr const double health_buff = 1;
    static constexpr const double damage_delay_buff = 0.8;
    static constexpr const double damage_buff = 0.25;

    static EnemyProperties _bProperties;
public:
    Bugol(float x, float y, b2World* world);
    
    virtual EnemyProperties& get_properties() override {return _bProperties;}
    virtual const EnemyProperties& get_properties() const override {return _bProperties;}

    virtual void level_up() override;

    ~Bugol() = default;
protected:

};