#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"
#include "collidable.hpp"
#include "player.hpp"

using namespace std;
using namespace sf;

Enemy::Enemy(
	b2World* world,
	sf::Vector2f position,
	sf::Vector2f size,
	b2BodyType body_type,
	const sf::Texture& texture,
	const EnemyProperties &properties
):
	collision(world, position, size, body_type, _categoryBits, _maskBits),
	texture(texture),
	_eProperties(properties)
{
	sprite.setTexture(texture);
	collision.data->damage_do = this->get_properties()._damage;
}

sf::Vector2f Enemy::getPosition() {
	return collision.getPosition();
}
sf::Vector2f Enemy::getSize() {
	auto bounds = sprite.getLocalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

void Enemy::updatePosition() {
	sprite.setPosition(collision.getPosition());
}

void Enemy::move(sf::Vector2f direction) {
	double rotation = atan2(direction.y, direction.x) * DEG_PER_RAD;
	collision.setVelocity(direction * (f32)_eProperties._agility);
}

void Enemy::move(Player& player) {
	Enemy::move(player.getPosition() - collision.getPosition());
}

void Enemy::updateHealth() {
	_eProperties._health -= collision.data->damage_take;
	collision.data->damage_take = 0;
}

const f64& Enemy::getHealth() {
	return _eProperties._health;
}

bool Enemy::isAlive() {
	return _eProperties._health > 0;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}

void Enemies::updateHealths() {
	for(auto& e: enemies) e->updateHealth();
	u32 i = 0;
	while(i < enemies.size()) {
		auto& enemy = enemies[i];
		if(!enemy || !enemy->isAlive()) {
			std::swap(enemies[i], enemies.back());
			enemies.pop_back();
			continue;
		}
		i += 1;
	}
}
