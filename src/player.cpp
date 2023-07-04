#include "player.hpp"

using namespace sf;
using namespace std;

Player::Player(
	b2World* world,
	sf::Vector2f position,
	sf::Vector2f size,
	b2BodyType body_type,
	std::string textureFile,
	PlayerProperties &&properties,
	WeaponType weaponType
):
	collision(world, position, size, body_type, _categoryBits, _maskBits),
	_pProperties(properties)
{
	texture.loadFromFile(textureFile);
	sprite.setTexture(texture);
	/*
	 * Allocate weapon 
	 */
	switch(weaponType) {
		case WeaponType::GUN:
			_weapon = new Gun(weaponProperties(5, 5, 10));
			break;
	}
}

Player::~Player() {
	delete _weapon;
}

void Player::updateHealth() {
	_pProperties._health -= collision.data->damage_take;
	collision.data->damage_take = 0;
}

sf::Vector2f Player::getPosition() {
	return collision.getPosition();
}
sf::Vector2f Player::getSize() {
	auto bounds = sprite.getLocalBounds();
	return sf::Vector2f(bounds.width, bounds.height);
}

void Player::updatePosition() {
	sprite.setPosition(collision.getPosition());
}

void Player::updateProjectiles() {
	auto& cartridge = _weapon->get_cartridge();
	int i = 0;
	while(i < cartridge.size()) {
		auto& proj = cartridge[i];
		if(!proj || !proj->isAlive()) {
			std::swap(cartridge[i], cartridge.back());
			cartridge.pop_back();
			continue;
		}
		i += 1;
	}
	for(auto& p: cartridge) p->updatePosition();
}

/*
 * Player movement configurations
 */
void Player::move() {
	/*
	 * Vertical and Horizontal movements
	 */
	auto isPressed = sf::Keyboard::isKeyPressed;
	using Key = sf::Keyboard::Key;

	sf::Vector2f v(0, 0);
	if(isPressed(Key::Up) || isPressed(Key::W))
		v.y -= _pProperties._agility;
	if(isPressed(Key::Down) || isPressed(Key::S))
		v.y += _pProperties._agility;
	if(isPressed(Key::Left) || isPressed(Key::A))
		v.x -= _pProperties._agility;
	if(isPressed(Key::Right) || isPressed(Key::D))
		v.x += _pProperties._agility;
	if(v.x || v.y) v *= (f32)_pProperties._agility / hypot(v.x, v.y);
	collision.setVelocity(v);
}

/*
 * Player attack configurations
 * Different types of projectiles are fired according to certain
 * requirements
 */

void Player::attack(sf::Vector2f target) {
	if(!Keyboard::isKeyPressed(Keyboard::Space)) return;
	_weapon->fire(
		ProjectileType::NORMAL,
		collision._world,
		collision.getPosition() + getSize() * 0.5f,
		sf::Vector2f(target.x, target.y)
	);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}