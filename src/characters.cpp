#include "characters.hpp"

/* VALOR SUJEITO A ALTERAÇÃO */
const int CHARACTER_TIMER = 1.1;

CVince::CVince(float x, float y, b2World* world, WeaponType weaponType)
    : Player(x, y, world, new Box(18, 40, 100.f), "characters_texture.png", PlayerProperties(health, defense, agility), weaponType), 
    _character(Vince) {
    _animation = new Animation(_sprite, _texture, CHARACTER_TIMER, sf::Vector2u(0,0), 32, 50, size_);
}

CNate::CNate(float x, float y, b2World* world, WeaponType weaponType)
    : Player(x, y, world, new Box(18, 40, 100.f), "characters_texture.png", PlayerProperties(health, defense, agility), weaponType), 
    _character(Nate) {
    _animation = new Animation(_sprite, _texture, CHARACTER_TIMER, sf::Vector2u(0,100), 32, 50, size_);
}

CMatt::CMatt(float x, float y, b2World* world, WeaponType weaponType)
    : Player(x, y, world, new Box(18, 40, 100.f), "characters_texture.png", PlayerProperties(health, defense, agility), weaponType), 
    _character(Matt) {
    _animation = new Animation(_sprite, _texture, CHARACTER_TIMER, sf::Vector2u(0,200), 32, 50, size_);
}

CDany::CDany(float x, float y, b2World* world, WeaponType weaponType)
    : Player(x, y, world, new Box(18, 40, 100.f), "characters_texture.png", PlayerProperties(health, defense, agility), weaponType), 
    _character(Dany) {
    _animation = new Animation(_sprite, _texture, CHARACTER_TIMER, sf::Vector2u(0,500), 32, 50, size_);
}

CJoy::CJoy(float x, float y, b2World* world, WeaponType weaponType)
    : Player(x, y, world, new Box(18, 40, 100.f), "characters_texture.png", PlayerProperties(health, defense, agility), weaponType), 
    _character(Joy) {
    _animation = new Animation(_sprite, _texture, CHARACTER_TIMER, sf::Vector2u(0,400), 32, 50, size_);
}

CLena::CLena(float x, float y, b2World* world, WeaponType weaponType)
    : Player(x, y, world, new Box(18, 40, 100.f), "characters_texture.png", PlayerProperties(health, defense, agility), weaponType), 
    _character(Lena) {
    _animation = new Animation(_sprite, _texture, CHARACTER_TIMER, sf::Vector2u(0,300), 32, 50, size_);
}