#ifndef CHARACTER_H
#define CHARACTER_H
#include "player.hpp"

enum Character { Vince, Nate, Matt, Lena, Joy, Dany };

/// Implementa a personagem Vince
class CVince: public Player {
private:
    Character _character;
    static constexpr const double health = 100;
    //range of 0-70% less damage
    static constexpr const double defense = 0.06;
    static constexpr const double agility = 1.5;
public:
    CVince(float x, float y, b2World* world, WeaponType weaponType);
    Character getCharacter() {return _character;}
    ~CVince() = default;
};

/// Implementa a personagem Nate
class CNate: public Player {
private:
    Character _character;
    static constexpr const double health = 150;
    //range of 0-70% less damage
    static constexpr const double defense = 0.11;
    static constexpr const double agility = 1.27;
public:
    CNate(float x, float y, b2World* world, WeaponType weaponType);
    Character getCharacter() {return _character;}
    ~CNate() = default;
};

/// Implementa a personagem Matt
class CMatt: public Player {
private:
    Character _character;
    static constexpr const double health = 100;
    //range of 0-70% less damage
    static constexpr const double defense = 0.01;
    static constexpr const double agility = 2.1;
public:
    CMatt(float x, float y, b2World* world, WeaponType weaponType);
    Character getCharacter() {return _character;}
    ~CMatt() = default;
};

/// Implementa a personagem Dany
class CDany: public Player {
private:
    Character _character;
    static constexpr const double health = 95;
    //range of 0-70% less damage
    static constexpr const double defense = 0.05;
    static constexpr const double agility = 1.80;
public:
    CDany(float x, float y, b2World* world, WeaponType weaponType);
    Character getCharacter() {return _character;}
    ~CDany() = default;
};

/// Implementa a personagem Joy
class CJoy: public Player {
private:
    Character _character;
    static constexpr const double health = 85;
    //range of 0-70% less damage
    static constexpr const double defense = 0.01;
    static constexpr const double agility = 2.2;
public:
    CJoy(float x, float y, b2World* world, WeaponType weaponType);
    Character getCharacter() {return _character;}
    ~CJoy() = default;
};

/// Implementa a personagem Lena
class CLena: public Player {
private:
    Character _character;
    static constexpr const double health = 125;
    //range of 0-70% less damage
    static constexpr const double defense = 0.13;
    static constexpr const double agility = 1.45;
public:
    CLena(float x, float y, b2World* world, WeaponType weaponType);
    Character getCharacter() {return _character;}
    ~CLena() = default;
};

#endif