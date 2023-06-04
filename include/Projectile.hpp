#pragma once
#include "GameUtilities.hpp"

/*
 * Here we set the properties of the projectile itself
 */
enum projectile {NORMAL};

class Projectile: public Collidable {
private:
    static constexpr const u32 _categoryBits = PROJECTILE;
    static constexpr const u32 _maskBits = STATIC|ENEMY;
public:
    Projectile(float x, float y, b2World* world, Shapeb2* shape, string texture, Color color, float damage);
protected:
    float _damage;
};

class NormalProj: public Projectile {
public:
    NormalProj(float x, float y, b2World* world, Shapeb2* shape, string texture, Color color, float damage);

};
