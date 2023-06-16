#include "projectile.hpp"
#include <cmath>
using namespace std;
using namespace sf;

Projectile::Projectile(float x, float y, b2World* world, Shapeb2* shape, string texture, Color color, float damage, float range)
    : Collidable(x, y, world, shape, b2_dynamicBody, texture, color, _categoryBits, _maskBits), _damage(damage), _range(range), _starting_position(Vector2f(x, y)) {
        _body->SetBullet(true);
        _data->damage_do = _damage;
}

double Projectile::_distance() {
    b2Vec2 pos = this->get_body()->GetPosition();
    Vector2f current_position = Vector2f(pos.x*PPM, pos.y*PPM);

    Vector2f tangentVector = current_position - _starting_position;
    double distance = sqrt(pow(tangentVector.x,2) + pow(tangentVector.y,2));
    
    return distance;
}


NormalProj::NormalProj(float x, float y, b2World* world, Shapeb2* shape, string texture, Color color, float damage, float range)
    : Projectile(x, y, world, shape, texture, color, damage, range) {}


