#include "../include/GameUtilities.hpp"

Enemy::Enemy(float x, float y, Shapeb2* shape, b2BodyType body_type, string texture, b2World* world, const enemyProperties &properties) 
    : Collidable(x, y, shape, body_type, texture, world), _eProperties(properties) {}

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