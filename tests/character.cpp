#include <doctest.h>
#include "characters.hpp"

TEST_SUITE("Characters Constructor Tests")
{
    b2World _world = b2World({0, 0});
    b2World* world = &_world;
    float x, y;
    TEST_CASE("Vince Constructor Test")
    {
        CVince vince = CVince(x, y, world, WeaponType::GUN);

        // Verifica se o tipo de personagem de Vince está correto
        CHECK(vince.getCharacter() == Vince);

        // Verifica se o tipo de arma de Vince está correto
        CHECK(vince.get_weapon()->_type == WeaponType::GUN);
    }

    TEST_CASE("Nate Constructor Test")
    {
        CNate nate = CNate(x, y, world, WeaponType::SHOTGUN);

        // Verifica se o tipo de personagem de Vince está correto
        CHECK(nate.getCharacter() == Nate);

        // Verifica se o tipo de arma de Vince está correto
        CHECK(nate.get_weapon()->_type == WeaponType::SHOTGUN);
    }

    TEST_CASE("Matt Constructor Test")
    {
        CMatt matt = CMatt(x, y, world, WeaponType::MACHINEGUN);

        // Verifica se o tipo de personagem de Vince está correto
        CHECK(matt.getCharacter() == Matt);

        // Verifica se o tipo de arma de Vince está correto
        CHECK(matt.get_weapon()->_type == WeaponType::MACHINEGUN);
    }

    TEST_CASE("Lena Constructor Test")
    {
        CLena lena = CLena(x, y, world, WeaponType::SHOTGUN);

        // Verifica se o tipo de personagem de Vince está correto
        CHECK(lena.getCharacter() == Lena);

        // Verifica se o tipo de arma de Vince está correto
        CHECK(lena.get_weapon()->_type == WeaponType::SHOTGUN);
    }

    TEST_CASE("Joy Constructor Test")
    {
        CJoy joy= CJoy(x, y, world, WeaponType::GUN);

        // Verifica se o tipo de personagem de Vince está correto
        CHECK(joy.getCharacter() == Joy);

        // Verifica se o tipo de arma de Vince está correto
        CHECK(joy.get_weapon()->_type == WeaponType::GUN);
    }

    TEST_CASE("Dany Constructor Test")
    {
        CDany dany = CDany(x, y, world, WeaponType::MACHINEGUN);

        // Verifica se o tipo de personagem de Vince está correto
        CHECK(dany.getCharacter() == Dany);

        // Verifica se o tipo de arma de Vince está correto
        CHECK(dany.get_weapon()->_type == WeaponType::MACHINEGUN);
    }
}