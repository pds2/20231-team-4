#pragma once
#include <cmath>
#include <random>
#include <memory>
#include <typeinfo>
#include <iostream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

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
void renderMovement(Collidable &character, RenderWindow &window);