#include "player.hpp"

using namespace sf;
using namespace std;

Player::Player(float x, float y, b2World* world, Shapeb2* shape, b2BodyType body_type, string texture, PlayerProperties &&pProperties, WeaponType weaponType) 
    : Collidable(x, y, world, shape, body_type, texture, Color::Black, _categoryBits, _maskBits), _pProperties(pProperties) {
    
    /*
     * Allocate weapon 
     */
    switch(weaponType) {
        case WeaponType::GUN:
            _weapon = new Gun(weaponProperties(10,10,0));
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
    
    long double angleRadians = atan2(targetVector.x, -targetVector.y);
    _body->SetTransform(_body->GetPosition(), -angleRadians);
    this->get_sprite().setRotation(angleRadians*DEG_PER_RAD);
}

/*
 * Player attack configurations
 * Different types of projectiles are fired according to certain
 * requirements
 */

void Player::_attack() {
    if(Keyboard::isKeyPressed(Keyboard::Space))
        _weapon->fire(ProjectileType::NORMAL, *this);
}

