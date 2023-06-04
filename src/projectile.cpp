#include "projectile.hpp"
using namespace std;
using namespace sf;

Projectile::Projectile(float x, float y, b2World* world, Shapeb2* shape, string texture, Color color, float damage)
	: Collidable(x, y, world, shape, b2_dynamicBody, texture, color, _categoryBits, _maskBits), _damage{damage} {
		_body->SetBullet(true);
	}

NormalProj::NormalProj(float x, float y, b2World* world, Shapeb2* shape, string texture, Color color, float damage)
	: Projectile(x, y, world, shape, texture, color, damage) {}