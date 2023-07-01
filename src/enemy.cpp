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

void Enemy::_move(Vector2f& flow_direction) {

    double angleRadians = atan2(flow_direction.x, -flow_direction.y);
    
    b2Vec2 velocityVector = b2Vec2(flow_direction.x*_eProperties._agility, flow_direction.y*_eProperties._agility);

    _body->SetLinearVelocity(velocityVector);
    _body->SetTransform(_body->GetPosition(), -angleRadians);
    
}