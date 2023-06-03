#include "collision.hpp"

struct Callback: b2ContactListener {
	void BeginContact(b2Contact* contact) override {
		bool* c;
		c = (bool*)contact->GetFixtureA()->GetUserData().pointer;
		*c = true;

		c = (bool*)contact->GetFixtureB()->GetUserData().pointer;
		*c = true;
	}
	void EndContact(b2Contact* contact) override {
		bool* c;
		c = (bool*)contact->GetFixtureA()->GetUserData().pointer;
		*c = false;

		c = (bool*)contact->GetFixtureB()->GetUserData().pointer;
		*c = false;
	}
};

CollisionObject::CollisionObject(
	b2World& world,
	b2BodyType type,
	sf::Vector2f p
): body(nullptr), collision(nullptr), sensor(nullptr)
{
	width = 50, height = 50;

	b2BodyDef bodyDef;
	bodyDef.position.Set(p.x * MPP, p.y * MPP);
	bodyDef.type = type;
	body = world.CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(width / 2 * MPP, height / 2 * MPP);
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0;
	colliding = std::make_shared<bool>(false);
	fixtureDef.userData.pointer = (uintptr_t)colliding.get();
	collision = body->CreateFixture(&fixtureDef);

	body->SetFixedRotation(false);
}

sf::Vector2f CollisionObject::getCenter() {
	b2Vec2 pos = body->GetPosition();
	return { pos.x * PPM, pos.y * PPM };
}
void CollisionObject::setVelocity(sf::Vector2f v) {
	body->SetLinearVelocity({ v.x * MPP, v.y * MPP });
}
void CollisionObject::updatePosition(sf::Transformable& t) {
	t.setOrigin(width / 2, height / 2);
	b2Vec2 pos = body->GetPosition();
	t.setPosition(pos.x * PPM, pos.y * PPM);
}

Callback callback = Callback();
CollisionWorld::CollisionWorld():
	world({0,0})
{
	world.SetContactListener(&callback);
}

CollisionObject CollisionWorld::spawnEnemy(sf::Vector2f p) {
	CollisionObject obj(world, b2_dynamicBody, p);
	objects.push_back(obj);
	return obj;
}

CollisionObject CollisionWorld::spawnObstacle(sf::Vector2f p) {
	CollisionObject obj(world, b2_staticBody, p);
	objects.push_back(obj);
	return obj;
}

void CollisionWorld::step(f32 seconds) {
	world.Step(seconds, 1, 10);
}

CollisionWorld::~CollisionWorld() {}