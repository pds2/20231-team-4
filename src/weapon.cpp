#include "weapon.hpp"
#include "collidable.hpp"
#include "player.hpp"
using namespace std;
using namespace sf;

/*
 * Weapon general configurations
 */
Weapon::Weapon(const weaponProperties &properties) 
    : _wProperties(properties), counter(_wProperties._fire_rate) {}

Weapon::~Weapon() {}

/* 
 * Gun configurations
 */
Gun::Gun(const weaponProperties &properties)
    : Weapon(properties) {}


/*
 * Gun firing configurations
 * Sets the point of origin and tragectory of each type of projectile
 */
void Gun::fire(ProjectileType projectile, b2World* world, sf::Vector2f position, sf::Vector2f target) {
    if(counter < _wProperties._fire_rate) {
		counter += 1;
		return;
	}
	switch(projectile) { 
		case ProjectileType::NORMAL:
			cartridge.push_back(make_shared<NormalProj>(
				world,
				position,
				target,
				_wProperties._damage,
				1000
			));
			break;
	}
	counter = 0;
}
