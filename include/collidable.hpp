/*
 * Here we build and configure bodies and their sprites
 */
#pragma once
#include <utility>
#include <iostream>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "types.hpp"

const u32 PPM = 64;
const f32 DEG_PER_RAD = 180.f/M_PI;

class CollisionData;

/*
 * To be used for collision management
 */
enum class CollidableType {
    STATIC = 0x0001,
    DYNAMIC = 0x002,
    PLAYER = 0x0004,
    ENEMY = 0x0008,
    PROJECTILE = 0x0010,
};
constexpr u32 operator|(u32 a, CollidableType b) {
    return a | (u32)b;
}

/*
 * Collidable struct
 */
struct Collidable {

    using enum CollidableType;
    Collidable(
		b2World *world,
		sf::Vector2f position,
		sf::Vector2f size,
		b2BodyType body_type=b2_staticBody,
		u32 categoryBits = 0|STATIC,
		u32 maskBits = 0|DYNAMIC|STATIC
	);

    // Physical properties
    b2World *_world;
    b2Body *_body;

    sf::Vector2f size;

    // Collision properties
    CollisionData *data;

	// Methods
	void setVelocity(sf::Vector2f velocity);
	sf::Vector2f getPosition() const;

	~Collidable();
};


/* 
 * Here we handle the collisions
 */
class CollisionData {
public:
    //The CollidableType of the body collided with
    bool colliding;
    u32 category;

    int damage_do;
    int damage_take;

    CollisionData() : colliding(0), category(0), damage_do(0), damage_take(0) {};
};

class MyContactListener: public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

};