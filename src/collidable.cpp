#include "collidable.hpp"

using namespace std;
using namespace sf;

/*
 * Building and configuring bodies
 */

Collidable::Collidable(
	b2World *world,
	sf::Vector2f position,
	sf::Vector2f size,
	b2BodyType body_type,
	u32 categoryBits,
	u32 maskBits
):
	_world(world),
	data(new CollisionData()),
	size(size)
{
	//Body definition
	position /= (f32)PPM;
	size /= (f32)PPM;
	b2BodyDef bodyDef;
	bodyDef.position.Set(position.x + size.x / 2, position.y + size.y / 2);
	bodyDef.type = body_type;
	bodyDef.fixedRotation = true;

	//Fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1;

	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2, size.y / 2);
	fixtureDef.shape = &shape;

	double shape_height = size.y;
	double shape_width = size.x;

	//Fixture filters for collision management
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.userData.pointer = (uintptr_t)data;

	//Body created
	_body = _world->CreateBody(&bodyDef);
	_body->CreateFixture(&fixtureDef);
}

sf::Vector2f Collidable::getPosition() const {
	b2Vec2 pos = _body->GetPosition();
	return sf::Vector2f(pos.x * PPM - size.x/2, pos.y * PPM - size.y/2);
}

void Collidable::setVelocity(sf::Vector2f velocity) {
	_body->SetLinearVelocity({velocity.x, velocity.y});
}

/*
 * Collidable Destructor
 */
Collidable::~Collidable() {
	_world->DestroyBody(_body);
	delete data;
}

/*
 * Collision handling
 */
void MyContactListener::BeginContact(b2Contact* contact) {
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	CollisionData* dataA = (CollisionData*) fixtureA->GetUserData().pointer;
	CollisionData* dataB = (CollisionData*) fixtureB->GetUserData().pointer;

	using enum CollidableType;
	auto hasCategory = [](b2Fixture* fixt, CollidableType category) {
		return (fixt->GetFilterData().categoryBits & (0|category)) != 0;
	};

	if(dataA) dataA->colliding = 1;
	if(dataB) dataB->colliding = 1;
	if(dataA && dataB) {
		// Enemy takes damage
		if(hasCategory(fixtureA, ENEMY) && hasCategory(fixtureB, PROJECTILE))
			dataA->damage_take += dataB->damage_do;
		if(hasCategory(fixtureB, ENEMY) && hasCategory(fixtureA, PROJECTILE))
			dataB->damage_take += dataA->damage_do;

		// Player takes damage
		if(hasCategory(fixtureA, PLAYER) && hasCategory(fixtureB, ENEMY))
			dataA->damage_take += dataB->damage_do;
		if(hasCategory(fixtureB, PLAYER) && hasCategory(fixtureA, ENEMY))
			dataB->damage_take += dataA->damage_do;
	}
}

void MyContactListener::EndContact(b2Contact* contact) {
	b2Fixture *fixtureA = contact->GetFixtureA();
	b2Fixture *fixtureB = contact->GetFixtureB();

	CollisionData* dataA = (CollisionData*) fixtureA->GetUserData().pointer;
	CollisionData* dataB = (CollisionData*) fixtureB->GetUserData().pointer;

	if(dataA) dataA->colliding = 0;
	if(dataB) dataB->colliding = 0;
}
