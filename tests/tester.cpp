#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "collidable.hpp"
#include "player.hpp"
#include "enemy.hpp"

TEST_CASE("01 - Criação de player") {
    Player* p1;
    CHECK_THROWS(p1 = new Frog(0,0, new b2World(b2Vec2(0,0)), WeaponType::GUN));

    delete p1;
}