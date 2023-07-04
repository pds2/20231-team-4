#pragma once
#include <iostream>
#include <box2d/box2d.h>
#include <string>
#include "collidable.hpp"
#include "player.hpp"
#include "types.hpp"

struct EnemyProperties {
	double _health;
	double _damage;
	double _defense;
	double _agility;

	EnemyProperties(double health, double damage, double defense, double agility) 
		: _health(health), _damage(damage), _defense(defense), _agility(agility) {};
	EnemyProperties(const EnemyProperties &properties) 
		: EnemyProperties(properties._health, properties._damage, properties._defense, properties._agility) {};
};

class Enemy: public sf::Drawable {
private:
	using enum CollidableType;
	static constexpr const u32 _categoryBits = 0|ENEMY|DYNAMIC;
	static constexpr const u32 _maskBits = 0|STATIC|PLAYER|PROJECTILE|ENEMY;
	Collidable collision;
	const sf::Texture& texture;
	sf::Sprite sprite;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Enemy(
		b2World* world,
		sf::Vector2f position,
		sf::Vector2f size,
		b2BodyType body_type,
		const sf::Texture& texture,
		const EnemyProperties &properties
	);
	void move(sf::Vector2f direction);
	void move(Player& player);
	void updatePosition();
	void updateHealth();
	const f64& getHealth();
	bool isAlive();
	sf::Vector2f getPosition();
	sf::Vector2f getSize();

	EnemyProperties& get_properties() {return _eProperties;}
	
protected:
	EnemyProperties _eProperties;
};

struct Enemies {
	std::vector<std::shared_ptr<Enemy>> enemies;
	int spawn_delay {0};
	void updateHealths();
};