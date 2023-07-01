#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"
#include "collidable.hpp"
#include "player.hpp"

using namespace std;
using namespace sf;

Enemy::Enemy(float x, float y, b2World* world, Shapeb2* shape, b2BodyType body_type, string texture, const EnemyProperties &properties) 
    : Collidable(x, y, world, shape, body_type, texture, Color::Black, _categoryBits, _maskBits), _eProperties(properties) {
        _data->damage_do = this->get_properties()._damage;
    }

void Enemy::_move(sf::Vector2f& direction) {

    double angleRadians = atan2(direction.x, -direction.y);
    
    b2Vec2 velocityVector = b2Vec2(direction.x*_eProperties._agility, direction.y*_eProperties._agility);

    _body->SetLinearVelocity(velocityVector);
    _body->SetTransform(_body->GetPosition(), -angleRadians);
    
}

void Enemy::_move(Player& player) {
    Vector2f thisPosition = _sprite.getPosition();
    Vector2f otherPosition = player.get_sprite().getPosition();

    Vector2f tangentVector = thisPosition - otherPosition;
    double norm = sqrt(pow(tangentVector.x, 2) + pow(tangentVector.y, 2));
    double angleRadians = atan2(tangentVector.x, -tangentVector.y);
    
    b2Vec2 velocityVector = b2Vec2((tangentVector.x*_eProperties._agility)/norm, (tangentVector.y*_eProperties._agility)/norm);

    _body->SetLinearVelocity(-velocityVector);
    _body->SetTransform(_body->GetPosition(), -angleRadians);
}