#ifndef COLLISION_H
#define COLLISION_H
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include "types.h"

/// Pixels Por Metro
static const f32 PPM = 64;
/// Metros Por Pixel
static const f32 MPP = 1 / PPM;
/// Radianos Por Grau
static const f32 RPD = M_PI / 180;
/// Graus Por Radiano
static const f32 DPR = 180 / M_PI;

enum class Collider {
	Player,
	Enemy,
	Bullet,
};

struct CollisionObject {
	private:
	f32 width, height;
	b2Body* body;
	b2Fixture* collision;
	b2Fixture* sensor;
	public:
	CollisionObject(
		b2World& world,
		b2BodyType type,
		sf::Vector2f position
	);
	sf::Vector2f getCenter();
	void setVelocity(sf::Vector2f pixels_per_second);
	void updatePosition(sf::Transformable& transformable);
};

class CollisionWorld {
	private:
	b2World world;
	std::vector<CollisionObject> objects;
	public:
	CollisionWorld();
	CollisionObject spawnEnemy(sf::Vector2f p);
	CollisionObject spawnObstacle(sf::Vector2f p);
	void step(f32 seconds);
};

#endif