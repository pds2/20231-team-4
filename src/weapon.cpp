#include "weapon.hpp"
#include "collidable.hpp"
#include "player.hpp"
using namespace std;
using namespace sf;

/*
 * Weapon general configurations
 */
Weapon::Weapon(const WeaponProperties &properties) 
    : _wProperties(properties), counter(_wProperties._fire_rate) {}


/*
 * Gun firing configurations
 * Sets the point of origin and tragectory of each type of projectile
 */
void Weapon::fire(ProjectileType projectile, Player &player) {
    if(counter >= (*this)._wProperties._fire_rate) {
        if(Keyboard::isKeyPressed(Keyboard::Space)) {
            float position_x = player.getPosition_().x;
            float position_y = player.getPosition_().y;

            switch(projectile) { 
                
                case ProjectileType::NORMAL:
                    cartridge.push_back(make_shared<NormalProj>(
                        player.getPosition_().x,
                        player.getPosition_().y,
                        player.get_world(),
                        new Circle(3,1),
                        (*this)._wProperties._damage,
                        500
                    ));

                    cartridge.back()->get_body()->SetTransform(
                        b2Vec2(position_x/PPM, position_y/PPM),
                        cartridge.back()->get_body()->GetAngle());
                    break;
            } 

            //Set projectile tragectory
            double rotation = player.getRotation_() *   
            static_cast<float>(M_PI)/180.f - static_cast<float>(M_PI)/2.f;
            double cosine = cos(rotation);
            double sine = sin(rotation);
            double norm = sqrt(pow(cosine, 2) + pow(sine, 2));
            Vector2f vec = Vector2f(cosine, 0) + Vector2f(0, sine); 
            velocity.x = vec.x * (*this)._wProperties._speed/norm;
            velocity.y = vec.y * (*this)._wProperties._speed/norm;

            cartridge.back()->set_velocity(b2Vec2(velocity.x, velocity.y));
            cartridge.back()->get_body()->SetLinearVelocity(cartridge.back()->get_velocity());
            counter = 0;
            int axis = 1;
            double rotationDeg = player.getRotation_()-90;
            sf::Sprite& _weapon_sprite = player.get_weapon_sprite();
            if(rotationDeg > -270 && rotationDeg < -90 && _weapon_sprite.getScale().y > 0) {
                _weapon_sprite.setScale(_weapon_sprite.getScale().x, -_weapon_sprite.getScale().y);
            }
            else if(rotationDeg > -90 && rotationDeg < 90 && _weapon_sprite.getScale().y < 0) {
                _weapon_sprite.setScale(_weapon_sprite.getScale().x, -_weapon_sprite.getScale().y);
            }
            _weapon_sprite.setRotation(rotationDeg);
        }
    } else {
        counter++;
    }
}
/* 
 * Gun configurations
 */
Gun::Gun()
    : Weapon(WeaponProperties(damage, speed, fireRate)) {
        _type = WeaponType::GUN;
    }


/*
 * Gun firing configurations
 * Sets the point of origin and tragectory of each type of projectile
 */
void Gun::fire(ProjectileType projectile, Player &player) {
    Weapon::fire(projectile, player);
}

/* 
 * Machine Gun configurations
 */
MachineGun::MachineGun()
    : Weapon(WeaponProperties(damage, speed, fireRate)) {
        _type = WeaponType::MACHINEGUN;
    }


/*
 * Machine Gun firing configurations
 * Sets the point of origin and tragectory of each type of projectile
 */
void MachineGun::fire(ProjectileType projectile, Player &player) {
    Weapon::fire(projectile, player);
}

/* 
 * ShotGun configurations
 */
ShotGun::ShotGun()
    : Weapon(WeaponProperties(damage, speed, fireRate)) {
        _type = WeaponType::SHOTGUN;
    }


/*
 * ShotGun firing configurations
 * Sets the point of origin and tragectory of each type of projectile
 */
void ShotGun::fire(ProjectileType projectile, Player &player) {
    Weapon::fire(projectile, player);
}
