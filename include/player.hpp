#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "collidable.hpp"
#include "weapon.hpp"

struct PlayerProperties {
    double _health;
    double _defense;
    double _agility;
    
    PlayerProperties(double health, double defense, double agility) 
        : _health(health), _defense(defense), _agility(agility) {}
    PlayerProperties(const PlayerProperties &properties) 
        : PlayerProperties(properties._health, properties._defense, properties._agility) {}
};

class Player: public sf::Drawable {
private:
    using enum CollidableType;
    static constexpr const u32 _categoryBits = 0|PLAYER|DYNAMIC;
    static constexpr const u32 _maskBits = 0|STATIC|PLAYER|ENEMY;

	sf::Texture texture;
	sf::Sprite sprite;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    Player(
		b2World* world,
		sf::Vector2f position,
		sf::Vector2f size,
		b2BodyType body_type,
		std::string texture,
		PlayerProperties &&properties,
		WeaponType weaponType
	);
	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	void updatePosition();
	void updateProjectiles();
	void updateHealth();
    Weapon* get_weapon() {return _weapon;}

    void move();
    void attack(sf::Vector2f target);

    PlayerProperties& get_properties() {return _pProperties;}

    void operator=(Player&& player);

    ~Player();
protected:
	Collidable collision;
    Weapon* _weapon;
    PlayerProperties _pProperties;
};
