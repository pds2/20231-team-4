#pragma once
#include <cmath>
#include <random>
#include <memory>
#include <typeinfo>
#include <iostream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "types.h"

/*
 * To be used for collision management
 */
enum _collidable {
    STATIC = 0x0001,
    PLAYER = 0x0002,
    ENEMY = 0x0004,
    PROJECTILE = 0x0008,
};

#include "Collidable.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "Projectile.hpp"
#include "Enemy.hpp"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define PPM 65.0F
#define DEG_PER_RAD 180/static_cast<float>(M_PI)

using namespace std;
using namespace sf;


//Only usable in main.cpp
void renderMovement(Collidable &obj, RenderWindow &window);

template<typename T>
void renderMovements(vector<shared_ptr<T>> &objs, RenderWindow &window) {
    for(auto it = objs.begin(); it != objs.end(); it++) {
        renderMovement(**it, window);
    }
}   
