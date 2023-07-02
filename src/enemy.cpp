#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"
#include "collidable.hpp"
#include "player.hpp"

using namespace std;
using namespace sf;

void Enemy::default_config() {
    _data->damage_do = this->get_properties()._damage;
}

Enemy::Enemy(float x, float y, b2World* world, Shapeb2* shape, string texture, EnemyProperties &&properties) 
    : Collidable(x, y, world, shape, b2_dynamicBody, texture, _categoryBits, _maskBits), _eProperties(properties) {
    default_config();
}
Enemy::Enemy(float x, float y, b2World* world, Shapeb2* shape, Color color, EnemyProperties &&properties) 
    : Collidable(x, y, world, shape, b2_dynamicBody, color, _categoryBits, _maskBits), _eProperties(properties) {
    default_config();
}

void Enemy::_move(sf::Vector2f& direction) {

    double angleRadians = atan2(direction.x, direction.y);
    
    b2Vec2 velocityVector = b2Vec2(direction.x*_eProperties._agility, direction.y*_eProperties._agility);

    _body->SetLinearVelocity(velocityVector);
    _body->SetTransform(_body->GetPosition(), angleRadians);
    
}

void Enemy::_move(Player& player) {
    Vector2f thisPosition = this->getPosition_();
    Vector2f otherPosition = player.getPosition_();

    Vector2f tangentVector = thisPosition - otherPosition;
    double norm = sqrt(pow(tangentVector.x, 2) + pow(tangentVector.y, 2));
    double angleRadians = atan2(tangentVector.x, -tangentVector.y);
    
    b2Vec2 velocityVector = b2Vec2((tangentVector.x*_eProperties._agility)/norm, (tangentVector.y*_eProperties._agility)/norm);

    _body->SetLinearVelocity(-velocityVector);
    _body->SetTransform(_body->GetPosition(), -angleRadians);
}