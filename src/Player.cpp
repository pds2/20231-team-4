#include "../include/GameUtilities.hpp"

Player::Player(float x, float y, Shapeb2* shape, b2BodyType body_type, string texture, b2World* world, playerProperties &&pProperties, weapon weaponType) 
    : Collidable(x, y, shape, body_type, texture, world), _pProperties(pProperties) {
    
    /*
     * Allocate weapon 
     */
    switch(weaponType) {
        case weapon::GUN:
            _weapon = new Gun(weaponProperties(10,5,10));
            break;
    }
}

Player::~Player() {
    delete _weapon;
}


/*
 * Player movement configurations
 */
void Player::_move(RenderWindow &window) {
    /*
     * Vertical and Horizontal movements
     */
    if(Keyboard::isKeyPressed(Keyboard::W))
        velocity.y = -_pProperties._agility;
    else if(Keyboard::isKeyPressed(Keyboard::S))
        velocity.y = _pProperties._agility;
    else 
        velocity.y = 0;
    if(Keyboard::isKeyPressed(Keyboard::A))
        velocity.x = -_pProperties._agility;
    else if(Keyboard::isKeyPressed(Keyboard::D))
        velocity.x = _pProperties._agility;
    else 
        velocity.x = 0;
    _body->SetLinearVelocity(velocity);

    /*
     * Rotate player according to mouse position
     */
    Vector2f playerPosition = _sprite.getPosition();
    Vector2i mousePosition = Mouse::getPosition(window);
    Vector2f targetVector = Vector2f(mousePosition.x, mousePosition.y) - playerPosition;
    
    float angleRadians = atan2(targetVector.x, -targetVector.y);
    _body->SetTransform(_body->GetPosition(), -angleRadians);
}

/*
 * Player attack configurations
 * Different types of projectiles are fired according to certain
 * requirements
 */

void Player::_attack() {
    if(Keyboard::isKeyPressed(Keyboard::Space))
        _weapon->fire(projectile::NORMAL, *this);
}

