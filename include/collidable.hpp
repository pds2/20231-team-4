/*
 * Here we build and configure bodies and their sprites
 */
#pragma once
#include <utility>
#include <iostream>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "types.hpp"

const u32 PPM = 64;
const f32 DEG_PER_RAD = 180.f/M_PI;

class CollisionData;

/*
 * To be used for collision management
 */
enum class CollidableType {
    STATIC = 0x0001,
    DYNAMIC = 0x002,
    PLAYER = 0x0004,
    ENEMY = 0x0008,
    PROJECTILE = 0x0010,
    XPORB = 0x0020,
    XPFIELD = 0x0040
};
constexpr u32 operator|(u32 a, CollidableType b) {
    return a | (u32)b;
}


/*
 * box2d shape structures
 */
struct Shapeb2 {
    b2BodyDef _BodyDef;
    b2FixtureDef _FixtureDef;
    double density;
    double height;
    double width;

    Shapeb2(double w, double h, double d): width(w), height(h), density(d) {}
    virtual b2Shape* getB2Shape() {return nullptr;}
};

struct Box: public Shapeb2 {
    b2PolygonShape box_Shape;

    Box(double w=0, double h=0, double d=0) : Shapeb2(w, h, d) {}
    virtual b2Shape* getB2Shape() override {return &box_Shape;}
};

struct Circle: public Shapeb2 {
    b2CircleShape circle_Shape;

    Circle(double r=0, double d=0) : Shapeb2(r, r, d) {}
    virtual b2Shape* getB2Shape() override {return &circle_Shape;}
};

struct Animation {
    sf::Texture& _texture;
    sf::Sprite& _sprite;

    sf::Vector2f body_size;

    int _width;
    int _height;
    sf::IntRect startRect;
    sf::IntRect currRect;
    sf::IntRect endRect;

    int _axis = 1;
    float _animationTimer;
    float _timer;
    
    Animation(sf::Sprite& sp, sf::Texture& tx, float animationTimer, 
                         sf::Vector2u end_f, int width, int height, 
                         sf::Vector2f bodySize)
    :_texture(tx), _sprite(sp),
     _animationTimer(animationTimer), _timer(0),
     startRect(0, 0, width, height), 
     endRect(end_f.x*width, end_f.y*height, width, height), currRect(startRect),
     _width(width), _height(height), body_size(bodySize) {

        sp.setTextureRect(startRect);

        _sprite.setScale(body_size.x/_width, body_size.y/_height);
        _sprite.setOrigin(_width/2.0f, _height/2.0f);
     }

    void axis_uptade(){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {_axis = -1;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {_axis = 1;}
    }

    void update(const float& dt) {
        _timer += 10.f * dt;
        if(_timer >= _animationTimer) {
            _timer = 0;
           
            if(currRect != endRect) {
                currRect.left += _width;
            } else {
                currRect = startRect;
            }

            axis_uptade();
            _sprite.setTextureRect(currRect);
            _sprite.setScale(_axis*body_size.x/_width, body_size.y/_height);
        }
    }

    void reset() {
        _timer = 0;
        currRect = startRect;
    }
};

class Collidable {
public:
    using enum CollidableType;
    Collidable(float x, float y, b2World* world, Shapeb2* shape, b2BodyType body_type, u32 categoryBits, u32 maskBits);
    Collidable(float x, float y, b2World* world, Shapeb2* shape, b2BodyType body_type, std::string texture, u32 categoryBits, u32 maskBits);
    Collidable(float x, float y, b2World* world, Shapeb2* shape, b2BodyType body_type, sf::Color color, u32 categoryBits, u32 maskBits);

    Animation* _animation;

    b2Body* get_body() {return _body;}
    const b2Body* get_body() const {return _body;}
    b2World* get_world() {return _world;}
    
    sf::Drawable& get_drawable() {
        if(_sfml_shape != nullptr)
            return *_sfml_shape;
        else    
            return _sprite;
    }
    void setPosition_(sf::Vector2f& new_position);
    void updateMovement(sf::RenderWindow &window);


    CollisionData* getCollisionData() {return _data;}
    sf::Vector2f& getPosition_() {return position_;}
    sf::Vector2f& getSize_() {return size_;}
    double& getRotation_() {return rotation_;}
    
    const CollisionData& getCollisionData() const {return *_data;}
    const sf::Vector2f& getPosition_() const {return position_;}
    const sf::Vector2f& getSize_() const {return size_;}
    const double& getRotation_() const {return rotation_;}

    ~Collidable();
protected:
    //Physical properties
    b2World *_world;
    b2Body *_body;
    Shapeb2 *_b2_shape;

    //Graphic properties
    sf::Vector2f position_;
    sf::Vector2f size_;
    double rotation_;
    sf::Texture _texture;
    sf::Sprite _sprite;

    sf::Shape *_sfml_shape; 

    //Collision properties
    CollisionData *_data;
};


/* 
 * Here we handle the collisions
 */
class CollisionData {
public:
    //The CollidableType of the body collided with
    bool colliding;
    u32 category;

    double damage_do;
    double damage_take;
    int ddelay_do;
    int ddelay_take;

    int counter;

    mutable bool collided;

    const CollisionData* other_data;

    CollisionData() : colliding(0), category(0), 
    damage_do(0), damage_take(0), counter(0), 
    ddelay_do(0), ddelay_take(0), collided(0) {};
};

class MyContactListener: public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

};


