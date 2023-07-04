#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <box2d/box2d.h>
#include "projectile.hpp"


/*
 * Weapon has a vector to projectiles 
 * it determines the fire rate and the speed of the projectiles 
 * as well as it's point of origin and tragectory  
 */

class Player;

enum class WeaponType {
    GUN
};

struct weaponProperties {
    double _damage;
    double _speed;
    double _fire_rate;

    weaponProperties(double damage, double speed, double fireRate)
        : _damage(damage), _speed(speed), _fire_rate(fireRate) {}
    weaponProperties(const weaponProperties &properties) 
        : weaponProperties(properties._damage, properties._speed, properties._fire_rate) {}
};

class Weapon {
public:
    Weapon(const weaponProperties &properties);

    virtual void fire(ProjectileType projectile, b2World* world, sf::Vector2f position, sf::Vector2f target) = 0;
    
    std::vector<std::shared_ptr<Projectile>>& get_cartridge() {return cartridge;}
	virtual ~Weapon();

protected:
    std::vector<std::shared_ptr<Projectile>> cartridge;
    weaponProperties _wProperties;
    b2Vec2 velocity;
    
    double counter;
};


class Gun: public Weapon {
public:
    Gun(const weaponProperties &properties);

    virtual void fire(
		ProjectileType projectile,
		b2World* world,
		sf::Vector2f position,
		sf::Vector2f target
	) override;
private:
};

