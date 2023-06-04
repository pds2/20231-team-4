#include "../include/GameUtilities.hpp"

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
void Gun::fire(projectile projectile, Player &player) {
    if(counter >= (*this)._wProperties._fire_rate) {
        float position_x = player.get_sprite().getPosition().x;
        float position_y = player.get_sprite().getPosition().y;

        switch(projectile) { 
            
            case projectile::NORMAL:
                cartridge.push_back(make_shared<NormalProj>(
                    player.get_sprite().getPosition().x*0.5,
                    player.get_sprite().getPosition().y*0.5,
                    player.get_world(),
                    new Circle(3, 10000),
                    "",
                    Color::Red,
                    (*this)._wProperties._damage
                ));

                cartridge.back()->get_body()->SetTransform(
                    b2Vec2(position_x/PPM, position_y/PPM),
                    cartridge.back()->get_body()->GetAngle());
                break;
        } 

        //Set projectile tragectory
        double rotation = player.get_sprite().getRotation()*static_cast<float>(M_PI)/180.f - static_cast<float>(M_PI)/2.f;
        double cosine = cos(rotation);
        double sine = sin(rotation);
        double norm = sqrt(pow(cosine, 2) + pow(sine, 2));
        Vector2f vec = Vector2f(cosine, 0) + Vector2f(0, sine); 
        velocity.x = vec.x*(*this)._wProperties._speed/norm;
        velocity.y = vec.y*(*this)._wProperties._speed/norm;

        cartridge.back()->get_body()->SetLinearVelocity(velocity);
        counter = 0;
    } else {
        counter++;
    }
}