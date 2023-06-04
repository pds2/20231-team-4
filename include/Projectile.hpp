#pragma once
#include "GameUtilities.hpp"

/*
 * Here we set the properties of the projectile itself
 */
enum projectile {NORMAL};

class Projectile: public Collidable {
public:
    Projectile(float x, float y, Shapeb2* shape, string texture, b2World* world, float damage);
protected:
    float _damage;

};

class NormalProj: public Projectile {
public:
    NormalProj(float x, float y, Shapeb2* shape, string texture, b2World* world, float damage);
private:
};
