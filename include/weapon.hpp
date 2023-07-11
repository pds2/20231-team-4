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
    GUN, MACHINEGUN, SHOTGUN
};

struct WeaponProperties {
public:
    double _damage;
    double _speed;
    double _fire_rate;

    WeaponProperties(double damage, double speed, double fireRate)
        : _damage(damage), _speed(speed), _fire_rate(fireRate) {}
    WeaponProperties(const WeaponProperties &properties) 
        : WeaponProperties(properties._damage, properties._speed, properties._fire_rate) {}
};

class Weapon {
public:
    WeaponType _type;

    Weapon(const WeaponProperties &properties);

    virtual void fire(ProjectileType projectile, Player &player);
    
    std::vector<std::shared_ptr<Projectile>>& get_cartridge() {return cartridge;}

protected:
    std::vector<std::shared_ptr<Projectile>> cartridge;
    WeaponProperties _wProperties;
    b2Vec2 velocity;
    
    double counter;
};


class Gun: public Weapon {
private:
    static constexpr const double damage = 2;
    static constexpr const double speed = 10;
    static constexpr const double fireRate = 5;

public:
    Gun();

    virtual void fire(ProjectileType projectile, Player &player) override;
};

class MachineGun: public Weapon {
private:
    static constexpr const double damage = 1;
    static constexpr const double speed = 15;
    static constexpr const double fireRate = 8;

public:
    MachineGun();

    virtual void fire(ProjectileType projectile, Player &player) override;
};

class ShotGun: public Weapon {
private:
    static constexpr const double damage = 4;
    static constexpr const double speed = 10;
    static constexpr const double fireRate = 50;

public:
    ShotGun();

    virtual void fire(ProjectileType projectile, Player &player) override;
};

