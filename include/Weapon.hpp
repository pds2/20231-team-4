#pragma once
#include "GameUtilities.hpp"
#include "Projectile.hpp"

/*
 * Weapon has a vector to projectiles 
 * it determines the fire rate and the speed of the projectiles 
 * as well as it's point of origin and tragectory  
 */

class Player;

enum weapon {GUN};

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

    virtual void fire(projectile projectile, Player &player) {return;}
    
    vector<shared_ptr<Projectile>>& get_cartridge() {return cartridge;}

protected:
    vector<shared_ptr<Projectile>> cartridge;
    weaponProperties _wProperties;
    b2Vec2 velocity;
    
    double counter;
};


class Gun: public Weapon {
public:
    Gun(const weaponProperties &properties);

    virtual void fire(projectile projectile, Player &player) override;
private:
};

