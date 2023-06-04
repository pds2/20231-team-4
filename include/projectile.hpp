#pragma once
#include <string>
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
    static constexpr const u32 _categoryBits = 0|PROJECTILE;
    static constexpr const u32 _maskBits = 0|STATIC|ENEMY;
public:
    Projectile(float x, float y, b2World* world, Shapeb2* shape, std::string texture, sf::Color color, float damage);
protected:
    float _damage;
};

class NormalProj: public Projectile {
public:
    NormalProj(float x, float y, b2World* world, Shapeb2* shape, std::string texture, sf::Color color, float damage);

};
