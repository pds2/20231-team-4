#include "collidable.hpp"

using namespace std;
using namespace sf;

/*
 * Building and configuring bodies
 */
Collidable::Collidable(float x, float y, 
           b2World* world, Shapeb2* shape, 
           b2BodyType body_type, u32 categoryBits, u32 maskBits)
    : _world(world), _data(new CollisionData) {
    /*
     * Creating body
     */
    _b2_shape = shape;

    //Body definition
    _b2_shape->_BodyDef.position.Set(x/PPM, y/PPM);
    _b2_shape->_BodyDef.type = body_type;

    //Fixture definition
    _b2_shape->_FixtureDef.density = shape->density;

        ///Fixture shape definition according to chosen shape (Box or Circle)
    if(auto box = dynamic_cast<Box*>(_b2_shape)) {
        _b2_shape->height /= 2*PPM;
        _b2_shape->width /= 2*PPM;

        box->box_Shape.SetAsBox(box->width, box->height);

        _b2_shape->_FixtureDef.shape = &box->box_Shape;
    }
    else if(auto circle = dynamic_cast<Circle*>(_b2_shape)) {
        _b2_shape->width = _b2_shape->height /= PPM;
        
        circle->circle_Shape.m_radius = _b2_shape->height;

        _b2_shape->_FixtureDef.shape = &circle->circle_Shape;
    }

        //Fixture filters for collision management
    _b2_shape->_FixtureDef.filter.categoryBits = categoryBits;
    _b2_shape->_FixtureDef.filter.maskBits = maskBits;

    _b2_shape->_BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(_data);

    //Body created
    _body = _world->CreateBody(&_b2_shape->_BodyDef);
    _body->CreateFixture(&_b2_shape->_FixtureDef);
    

    //angular velocity equal to 0 to stop body from spining whenever it collides
    _body->SetAngularVelocity(0);


    Vector2f bodySize (_b2_shape->width*2*PPM, _b2_shape->height*2*PPM);
    Vector2f bodyPosition (_body->GetPosition().x*PPM, _body->GetPosition().y*PPM);

    position_ = bodyPosition;
    size_ = bodySize;
}

Collidable::Collidable(float x, float y, 
                    b2World* world, Shapeb2* shape,
                    b2BodyType body_type, string texture, 
                    u32 categoryBits, u32 maskBits)
    : Collidable(x, y, world, shape, body_type, categoryBits, maskBits) {

    rotation_ = -1*_body->GetAngle() * DEG_PER_RAD;

    //Obtaining and setting texture path
    _defaultTexture.loadFromFile("assets/" + texture);
    _sprite.setTexture(_defaultTexture);

    //Setting sprite's scale
    Vector2f spriteSize (_sprite.getTexture()->getSize());
    
    _sprite.setScale(size_.x/spriteSize.x, size_.y/spriteSize.y);
        
    //Setting sprites's origin and initial rotation and position
    _sprite.setOrigin(_sprite.getTexture()->getSize().x/2.0f, _sprite.getTexture()->getSize().y/2.0f);
    _sprite.setRotation(rotation_);
    _sprite.setPosition(position_.x, position_.y);

    _sfml_shape = nullptr;
}

Collidable::Collidable(float x, float y, 
                    b2World* world, Shapeb2* shape,
                    b2BodyType body_type, Color color, 
                    u32 categoryBits, u32 maskBits)
    : Collidable(x, y, world, shape, body_type, categoryBits, maskBits) {
    
    rotation_ = -1*_body->GetAngle() * DEG_PER_RAD;

    if(auto box = dynamic_cast<Box*>(_b2_shape)) {
        _sfml_shape = new RectangleShape(); 
        if(auto rectangle = dynamic_cast<RectangleShape*>(_sfml_shape)) {
            rectangle->setSize(Vector2f(size_.x, size_.y));
            rectangle->setOrigin(size_.x*0.5, size_.y*0.5);
            rectangle->setPosition(position_.x, position_.y);
            rectangle->setRotation(rotation_);
            rectangle->setFillColor(color); 
        }

    } else if(auto circle = dynamic_cast<Circle*>(_b2_shape)) {
        _sfml_shape = new CircleShape();
        if(auto circle = dynamic_cast<CircleShape*>(_sfml_shape)) {
            circle->setRadius(size_.x/2);
            circle->setOrigin(circle->getRadius(), circle->getRadius());
            circle->setPosition(position_.x, position_.y);
            circle->setRotation(rotation_);
            circle->setFillColor(color);
        }
    }
}

/*
 * Collidable Destructor
 */
Collidable::~Collidable() {
    _world->DestroyBody(_body);
    delete _b2_shape;
    delete _sfml_shape;
    delete _data;
}

void Collidable::setPosition_(sf::Vector2f& new_position) {
   
    if(_sfml_shape != nullptr) {
        _sfml_shape->setPosition(new_position);
        _sfml_shape->setRotation(rotation_);
    } else {
        _sprite.setPosition(new_position);
        _sprite.setRotation(rotation_);
    }
    this->position_ = new_position;
}

void Collidable::updateMovement(sf::RenderWindow &window) {
	sf::Vector2f new_position (_body->GetPosition().x*PPM, 
							   _body->GetPosition().y*PPM);
	rotation_ = -1*_body->GetAngle() * DEG_PER_RAD;

	this->setPosition_(new_position);
}

/*
 * Collision handling
 */

void MyContactListener::BeginContact(b2Contact* contact) {
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();
   
    CollisionData* dataA = reinterpret_cast<CollisionData*>(bodyA->GetUserData().pointer);
    CollisionData* dataB = reinterpret_cast<CollisionData*>(bodyB->GetUserData().pointer);

    if(dataA) {
        dataA->category = (u32) fixtureB->GetFilterData().categoryBits;
        dataA->damage_take = dataB->damage_do;
        dataA->colliding = 1;
    }
    if(dataB) {
        dataB->category = (u32) fixtureA->GetFilterData().categoryBits;
        dataB->damage_take = dataA->damage_do;
        dataB->colliding = 1;
    }
}


void MyContactListener::EndContact(b2Contact* contact) {
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();
    
    //CollisionData* dataA = reinterpret_cast<CollisionData*>(bodyA->GetUserData().pointer);
    //CollisionData* dataB = reinterpret_cast<CollisionData*>(bodyB->GetUserData().pointer);

    bodyB->SetAngularVelocity(0);
    bodyB->SetLinearVelocity(b2Vec2(0,0));
}