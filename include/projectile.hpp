#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "collidable.hpp"

/*
 * Here we set the properties of the projectile itself
 */
enum class ProjectileType {
    NORMAL
};

class Projectile: public Collidable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|PROJECTILE|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|ENEMY;

    b2Vec2 velocity;
    sf::Vector2f _starting_position;

    void default_config();
public:
    Projectile(float x, float y, b2World* world, Shapeb2* shape, std::string texture, float damage, float range);
    Projectile(float x, float y, b2World* world, Shapeb2* shape, sf::Color texture, float damage, float range);

    const b2Vec2& get_velocity() const {return velocity;}
    void set_velocity(b2Vec2 new_velocity);
    float get_damage() {return _damage;}
    float get_range() {return _range;}

    double _distance();

protected:
    float _damage;
    float _range;
};

class NormalProj: public Projectile {
public:
    NormalProj(float x, float y, b2World* world, Shapeb2* shape, float damage, float range);

};
