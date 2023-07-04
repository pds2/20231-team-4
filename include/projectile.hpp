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

class Projectile: public sf::Drawable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|PROJECTILE|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|ENEMY;


    sf::Vector2f starting_position;
public:
    Projectile(
		b2World* world,
		sf::Vector2f position,
		sf::Vector2f target,
		sf::Vector2f size,
		float damage,
		float range
	);

	sf::Vector2f getPosition() const;
	bool isAlive();
	virtual void updatePosition() = 0;

    double distance();

protected:
	Collidable collision;
    float damage;
    float range;
};

class NormalProj: public Projectile {
private:
	sf::CircleShape circle;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
    NormalProj(
		b2World* world,
		sf::Vector2f position,
		sf::Vector2f target,
		float damage,
		float range
	);
	void updatePosition() override;
};
