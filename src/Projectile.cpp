#include "../include/GameUtilities.hpp"

Projectile::Projectile(float x, float y, Shapeb2* shape, string texture, b2World* world, float damage)
    : Collidable(x, y, shape, b2_dynamicBody, texture, world), _damage{damage} {}


NormalProj::NormalProj(float x, float y, Shapeb2* shape, string texture, b2World* world, float damage)
    : Projectile(x, y, shape, texture, world, damage) {}