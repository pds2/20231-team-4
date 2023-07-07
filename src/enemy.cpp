#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"
#include "collidable.hpp"
#include "player.hpp"

using namespace std;
using namespace sf;

void Enemy::default_config() {
    _data->damage_do = get_properties()._damage;
    _data->ddelay_do = get_properties().damage_delay;
}

Enemy::Enemy(float x, float y, b2World* world, Shapeb2* shape, string texture, EnemyProperties &&properties) 
    : Collidable(x, y, world, shape, b2_dynamicBody, texture, _categoryBits, _maskBits), _eProperties(properties), _gui(*this) {
    default_config();
}
Enemy::Enemy(float x, float y, b2World* world, Shapeb2* shape, Color color, EnemyProperties &&properties) 
    : Collidable(x, y, world, shape, b2_dynamicBody, color, _categoryBits, _maskBits), _eProperties(properties), _gui(*this) {
    default_config();
}

void Enemy::_move(Vector2f& direction, RenderWindow& window) {

    double angleRadians = atan2(direction.x, direction.y);
    
    velocity = b2Vec2(direction.x*_eProperties._agility, direction.y*_eProperties._agility);

    _body->SetLinearVelocity(velocity);
    _body->SetTransform(_body->GetPosition(), angleRadians);

    updateMovement(window);
    this->_gui.updateHPBar();
}

void Enemy::_move(Player& player, RenderWindow& window) {
    Vector2f thisPosition = this->getPosition_();
    Vector2f otherPosition = player.getPosition_();

    Vector2f tangentVector = thisPosition - otherPosition;
    double norm = sqrt(pow(tangentVector.x, 2) + pow(tangentVector.y, 2));
    double angleRadians = atan2(tangentVector.x, -tangentVector.y);
    
    velocity = b2Vec2((tangentVector.x*_eProperties._agility)/norm, (tangentVector.y*_eProperties._agility)/norm);

    _body->SetLinearVelocity(-velocity);
    _body->SetTransform(_body->GetPosition(), -angleRadians);

    this->_gui.updateHPBar();
    updateMovement(window);
}

void EnemyGUI::initHBar() {
    sf::Vector2f size(20.f, 1.f);
    this->hpBarBack.setSize(size);
    this->hpBarBack.setFillColor(sf::Color(250,50,50, 200));
    this->hpBarBack.setOrigin(size/2.f);
    this->hpBarBack.setPosition(enemy_.getPosition_().x, 
    (enemy_.getPosition_().y + enemy_.getSize_().y));

    this->hpBarInner.setSize(sf::Vector2f(20.f,1.f));
    this->hpBarInner.setFillColor(sf::Color(20,250,20, 200));
    this->hpBarInner.setOrigin(size/2.f);
    this->hpBarInner.setPosition(this->hpBarBack.getPosition());
}

EnemyGUI::EnemyGUI(const Enemy& player) : enemy_(player) {
    this->initHBar();
}

void EnemyGUI::updateHPBar() {
    sf::Vector2f size(20.f*(enemy_.get_properties()._health/enemy_.get_properties().get_default_health()), 1.f);
    
    this->hpBarBack.setPosition(enemy_.getPosition_().x, 
    (enemy_.getPosition_().y + enemy_.getSize_().y));
    this->hpBarInner.setPosition(this->hpBarBack.getPosition());
    this->hpBarInner.setSize(size);
}

void EnemyGUI::renderGUI(ZRenderer& renderer) {
    renderer.insert(0, this->hpBarBack);
    renderer.insert(0, this->hpBarInner);
}

XpOrb::XpOrb(Enemy& e): xp(e.get_properties()._xp), 
    Collidable(e.getPosition_().x, e.getPosition_().y, e.get_world(), 
    new Circle(3.f, 0), b2_staticBody, sf::Color(100,250,100,200), 
    _categoryBits, _maskBits) {

    _data->damage_do = xp;
}

void XpOrb::renderOrb(ZRenderer& renderer) {
    renderer.insert(0, this->get_drawable());
}

Enemies::Enemies(int delay) : spawn_delay(delay), counter(0) {}

void Enemies::handleEnemies() {
    auto it = enemies_.begin();
	while(it != enemies_.end()) {
		if(auto enemy = *it) {
            if(enemy->getCollisionData()->counter < enemy->get_properties().damage_delay)   
                enemy->getCollisionData()->counter++;

            if(enemy->getCollisionData()->colliding) {
                switch(enemy->getCollisionData()->category) {
                    
                    case ((u32)CollidableType::PROJECTILE|(u32)CollidableType::DYNAMIC): 
                        enemy->get_properties()._health -= enemy->getCollisionData()->damage_take;
                        
                        if(enemy->get_properties()._health <= 0) {
                            xpOrbs_.push_back(std::make_shared<XpOrb>(*enemy));
                            it->reset();
                        }
                        break;
                    
                    case ((u32)CollidableType::PLAYER|(u32)CollidableType::DYNAMIC):
                        if(enemy->getCollisionData()->counter >= enemy->get_properties().damage_delay)
                            enemy->getCollisionData()->counter = 0;
                            enemy->getCollisionData()->collided = false;
                        break;
                }
                if(*it == nullptr) {
                    it = enemies_.erase(it);
                } else 
                    it++;
            
            } else {
                it++;
            }
        } else
            it = enemies_.erase(it); 
	}
}

void Enemies::spawnEnemy(sf::RenderWindow& window, b2World& world, sf::View& camera, const Player& player) {
    if(counter >= spawn_delay && enemies_.size() < 300) {
        window.setView(camera);
        sf::Vector2f pp = player.getPosition_();
        sf::Vector2u ws = window.getSize();

        auto isInsidePlayerBounds = [&](sf::Vector2f pos) -> bool {
            double distance = std::sqrt(std::pow(pos.x - pp.x, 2) + std::pow(pos.y - pp.y, 2));
            return distance > 250;
        };

        auto isOutsideWindowBounds = [&](sf::Vector2f pos) -> bool {
            return (pos.x < (100) || pos.x > (ws.x) || pos.y < (100) || pos.y > (ws.y));
        };

        sf::Vector2f pos (100 + rand()%(ws.x), 100 + rand()%(ws.y));
        while(isInsidePlayerBounds(pos) || isOutsideWindowBounds(pos))
            pos = Vector2f(100 + rand()%(ws.x), 100 + rand()%(ws.y));

        enemies_.push_back(std::make_shared<Enemy>(pos.x, pos.y, &world, new Box(8, 8, 1.f),  
        "bugol.png", EnemyProperties(30,10,100,10, 1+rand()%1, std::make_pair(1, 5))));
        counter = 0;
    } else
        counter++;
}

void Enemies::handleOrbs() {
    auto it = xpOrbs_.begin();
    while(it != xpOrbs_.end()) {
        if(auto orb = *it) {
            if(orb->getCollisionData()->get_colliding() && 
			   orb->getCollisionData()->get_category() == 
               (u32) CollidableType::XPFIELD) {
                it->reset();
                it = xpOrbs_.erase(it);
            } else  
                it++;
        }
    }
}
