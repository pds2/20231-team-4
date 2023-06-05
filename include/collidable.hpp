/*
 * Here we build and configure bodies and their sprites
 */
#pragma once
#include <utility>
#include <iostream>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "types.h"

const u32 PPM = 64;
const f32 DEG_PER_RAD = 180.f/M_PI;

/*
 * To be used for collision management
 */
enum class CollidableType {
    STATIC = 0x0001,
    DYNAMIC = 0x002,
    PLAYER = 0x0004,
    ENEMY = 0x0008,
    PROJECTILE = 0x0010,
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

    Shapeb2(double d): density(d) {}
    virtual b2Shape* getB2Shape() {return nullptr;}
};

struct Box: public Shapeb2 {
    b2PolygonShape box_Shape;
    double width;
    double height;

    Box(double w=0, double h=0, double d=0) : width(w), height(h), Shapeb2(d) {}
    virtual b2Shape* getB2Shape() override {return &box_Shape;}
};

struct Circle: public Shapeb2 {
    b2CircleShape circle_Shape;
    double radius;

    Circle(double r=0, double d=0) : radius(r), Shapeb2(d) {}
    virtual b2Shape* getB2Shape() override {return &circle_Shape;}
};


/*
 * Collidable class
 */

class Collidable {
public:

    using enum CollidableType;
    Collidable(float x=0, float y=0, b2World *world=nullptr, Shapeb2* shape=nullptr, b2BodyType body_type=b2_staticBody, std::string texture="", sf::Color color = sf::Color::Black, u32 categoryBits = 0|STATIC, u32 maskBits = 0|DYNAMIC|STATIC);

    b2Body* get_body() {return _body;}
    b2World* get_world() {return _world;}

    sf::Shape* get_sfml_shape() {return _sfml_shape;}
    sf::Sprite& get_sprite() {return _sprite;}


    ~Collidable();
protected:
    //Physical properties
    b2World *_world;
    b2Body *_body;
    Shapeb2 *_b2_shape;

    //Graphic properties
    sf::Sprite _sprite;
    sf::Texture _startingTexture;
    sf::Shape *_sfml_shape; 

    //Collision properties
    bool collided;
};


/* 
 * Here we handle the collisions
 */

class MyContactListener: public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
};