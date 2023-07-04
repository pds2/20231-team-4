#include <doctest.h>
#include "assets.hpp"
#include "projectile.hpp"
#include "enemy.hpp"
#include "collidable.hpp"

TEST_CASE("Enemy takes damage") {
	b2World world({0, 0});

	NormalProj proj(
		&world,
		{0, 0},
		{64, 64},
		10,
		1000
	);

	sf::Texture texture;
	Enemy enemy(
		&world,
		{64, 64},
		{64, 64},
		b2_dynamicBody,
		texture,
		EnemyProperties(1, 0, 0, 0)
	);

	CHECK(enemy.isAlive());
	for(u32 i = 0; i < 8; i += 1) {
		world.Step(1/60.0f, 6, 3);
		enemy.updateHealth();
	}
	CHECK(!enemy.isAlive());
}

TEST_CASE("Player takes damage") {

}