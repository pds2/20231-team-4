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


/* 
 * Gun configurations
 */
Gun::Gun(const weaponProperties &properties)
    : Weapon(properties) {}


/*
 * Gun firing configurations
 * Sets the point of origin and tragectory of each type of projectile
 */
void Gun::fire(ProjectileType projectile, Player &player) {
    if(counter >= (*this)._wProperties._fire_rate) {
        float position_x = player.getPosition_().x;
        float position_y = player.getPosition_().y;

        switch(projectile) { 
            
            case ProjectileType::NORMAL:
                cartridge.push_back(make_shared<NormalProj>(
                    player.getPosition_().x,
                    player.getPosition_().y,
                    player.get_world(),
                    new Circle(3,1),
                    Color::Blue,
                    (*this)._wProperties._damage,
                    1000
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
    } else {
        counter++;
    }
}
