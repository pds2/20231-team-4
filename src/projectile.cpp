#include "projectile.hpp"
#include <cmath>
using namespace std;
using namespace sf;

Projectile::Projectile(
	b2World* world,
	sf::Vector2f position,
	sf::Vector2f target,
	sf::Vector2f size,
	float damage,
	float range
):
	collision(world, position, size, b2_dynamicBody, _categoryBits, _maskBits),
	damage(damage),
	range(range),
	starting_position(position)
{
	collision._body->SetBullet(true);
	collision.data->damage_do = damage;

	sf::Vector2f v = target - position;
	if(v.x || v.y) v *= 10 / hypot(v.x, v.y);
	collision.setVelocity(v);
}

double Projectile::distance() {
	sf::Vector2f current_position = collision.getPosition();
	Vector2f tangentVector = current_position - starting_position;
	return hypot(tangentVector.x, tangentVector.y);
}

sf::Vector2f Projectile::getPosition() const {
	return collision.getPosition();
}

bool Projectile::isAlive() {
	return !collision.data->colliding &&
		collision._body->GetLinearVelocity() != b2Vec2(0,0) &&
		distance() < range;
}

NormalProj::NormalProj(
	b2World* world,
	sf::Vector2f position,
	sf::Vector2f target,
	float damage,
	float range
):
	Projectile(world, position, target, {4, 4}, damage, range),
	circle(2)
{}

void NormalProj::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(circle, states);
}

void NormalProj::updatePosition() {
	auto pos = collision.getPosition();
	circle.setPosition(collision.getPosition());
	circle.setOrigin({ 1, 1 });
}