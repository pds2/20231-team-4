#include "collidable.hpp"

using namespace std;
using namespace sf;

/*
 * Building and configuring bodies
 */

Collidable::Collidable(float x, float y, b2World* world, Shapeb2* shape, b2BodyType body_type, string texture, Color color, u32 categoryBits, u32 maskBits): _world(world), _data(new CollisionData) {
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
    auto box = dynamic_cast<Box*>(_b2_shape);
    auto circle = dynamic_cast<Circle*>(_b2_shape);

    double shape_height;
    double shape_width;

    if(box) {
        shape_height = box->height/2/PPM;
        shape_width = box->width/2/PPM;

        box->box_Shape.SetAsBox(shape_width, shape_height);

        _b2_shape->_FixtureDef.shape = &box->box_Shape;
    }
    else if(circle) {
        shape_height = shape_width = circle->radius/PPM;
        
        circle->circle_Shape.m_radius = shape_height;

        _b2_shape->_FixtureDef.shape = &circle->circle_Shape;
    }

        //Fixture filters for collision management
    _b2_shape->_FixtureDef.filter.categoryBits = categoryBits;
    _b2_shape->_FixtureDef.filter.maskBits = maskBits;

    _b2_shape->_BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(_data);

    //Body created
    _body = _world->CreateBody(&_b2_shape->_BodyDef);
    _body->CreateFixture(&_b2_shape->_FixtureDef);
    

    //angular velocity equal to 0 to stop player from spining whenever it collides
    _body->SetAngularVelocity(0);


    /*
     * Creating sfml shape/sprite: 
     * case texture is defined a sprite is created
     * else a sfml shape (rectangle/circle) is created
     */

    double bodySize_x = shape_width*2*PPM;
    double bodySize_y = shape_height*2*PPM;
    double bodyPosition_x = _body->GetPosition().x*PPM;
    double bodyPosition_y = _body->GetPosition().y*PPM;
    double rotation = -1*_body->GetAngle() * DEG_PER_RAD;

    position_ = Vector2f(bodyPosition_x, bodyPosition_y);
    size_ = Vector2f(bodySize_x, bodySize_y);

    if(!texture.empty()) {

        //Obtaining and setting texture path
        _startingTexture.loadFromFile("assets/" + texture);
        _sprite.setTexture(_startingTexture);

        //Setting sprite's scale
        double spriteSize_x = _sprite.getTexture()->getSize().x;
        double spriteSize_y = _sprite.getTexture()->getSize().y;
        _sprite.setScale(bodySize_x/spriteSize_x, bodySize_y/spriteSize_y);
        
        //Setting sprites's origin and initial rotation and position
        _sprite.setOrigin(_sprite.getTexture()->getSize().x/2.0f, _sprite.getTexture()->getSize().y/2.0f);
        _sprite.setRotation(rotation);
        _sprite.setPosition(bodyPosition_x, bodyPosition_y);

        _sfml_shape = nullptr;
    } else {
        //Assigns _sfml_shape a rectangle or circle shape according to box2d body's shape

        if(box) {
            _sfml_shape = new RectangleShape(); 
            if(auto rectangle = dynamic_cast<RectangleShape*>(_sfml_shape)) {
                rectangle->setSize(Vector2f(bodySize_x, bodySize_y));
                rectangle->setOrigin(bodySize_x*0.5, bodySize_y*0.5);
                rectangle->setPosition(bodyPosition_x, bodyPosition_y);
                rectangle->setRotation(rotation);
                rectangle->setFillColor(color);
            }

        }
        else if(circle) {
            _sfml_shape = new CircleShape();
            if(auto circle = dynamic_cast<CircleShape*>(_sfml_shape)) {
                circle->setRadius(bodySize_x/2);
                circle->setOrigin(circle->getRadius(), circle->getRadius());
                circle->setPosition(bodyPosition_x, bodyPosition_y);
                circle->setRotation(rotation);
                circle->setFillColor(color);
            }
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

void Collidable::setPosition_(sf::Vector2f& new_position, double rotation) {
   
    if(_sfml_shape != nullptr) {
        _sfml_shape->setPosition(new_position);
        _sfml_shape->setRotation(rotation);
    }
    else {
        _sprite.setPosition(new_position);
        _sprite.setRotation(rotation);
    }
    this->position_ = new_position;
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