#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include "enemy.hpp"
#include "collidable.hpp"
#include "player.hpp"

using namespace std;
using namespace sf;

Enemy::Enemy(float x, float y, b2World* world, Shapeb2* shape, string texture) 
    : Collidable(x, y, world, shape, b2_dynamicBody, texture, _categoryBits, _maskBits), _gui(*this) {
}
Enemy::Enemy(float x, float y, b2World* world, Shapeb2* shape, Color color) 
    : Collidable(x, y, world, shape, b2_dynamicBody, color, _categoryBits, _maskBits), _gui(*this) {
}

void Enemy::_move(Vector2f& direction, RenderWindow& window) {

    double angleRadians = atan2(direction.x, direction.y);
    
    velocity = b2Vec2(direction.x*get_properties()._agility, direction.y*get_properties()._agility);

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
    
    velocity = b2Vec2((tangentVector.x*get_properties()._agility)/norm, (tangentVector.y*get_properties()._agility)/norm);

    _body->SetLinearVelocity(-velocity);
    _body->SetTransform(_body->GetPosition(), -angleRadians);

    this->_gui.updateHPBar();
    updateMovement(window);

    if(velocity != b2Vec2(0,0))
        _animation->update(1);
    else
        _animation->reset();
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
    sf::Vector2f size(20.f*(enemy_._health/enemy_.get_properties()._health), 1.f);
    
    this->hpBarBack.setPosition(enemy_.getPosition_().x, 
    (enemy_.getPosition_().y + enemy_.getSize_().y));
    this->hpBarInner.setPosition(this->hpBarBack.getPosition());
    this->hpBarInner.setSize(size);
}

void EnemyGUI::renderGUI(ZRenderer& renderer) {
    renderer.insert(0, this->hpBarBack);
    renderer.insert(0, this->hpBarInner);
}

XpOrb::XpOrb(Enemy& e): xp(e._xp), 
    Collidable(e.getPosition_().x, e.getPosition_().y, e.get_world(), 
    new Circle(2.f, 0), b2_staticBody, "xporbs.png", _categoryBits, _maskBits) {
    _data->damage_do = xp;
    _animation = new Animation(_sprite, _texture, 30, sf::Vector2u(1,0), 16, 16, size_);
}

void XpOrb::renderOrb(ZRenderer& renderer) {
    renderer.insert(0, this->get_drawable());
}

Enemies::Enemies(int delay) : spawn_delay(delay), counter(delay) {}

void Enemies::handleEnemies(TextTagSystem& tts) {
    auto it = enemies_.begin();
	while(it != enemies_.end()) {
		if(auto enemy = *it) {
            sf::Vector2f ep = enemy->getPosition_();
            sf::Vector2f es = enemy->getSize_();

            if(enemy->getCollisionData()->counter < enemy->get_properties().damage_delay)   
                enemy->getCollisionData()->counter++;

            if(enemy->getCollisionData()->colliding) {
                switch(enemy->getCollisionData()->category) {
                    
                    case ((u32)CollidableType::PROJECTILE|(u32)CollidableType::DYNAMIC): 
                        enemy->_health -= enemy->getCollisionData()->damage_take;
                        enemy->getCollisionData()->other_data->collided = true;
                        
                        tts.addTextTag(PLAYERDAMAGE_TAG, ep.x-es.x, ep.y-es.y, "-", enemy->getCollisionData()->damage_take, "");
                        if(enemy->_health <= 0) {
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
            return distance > 500;
        };

        auto isOutsideWindowBounds = [&](sf::Vector2f pos) -> bool {
            return (pos.x < (100) || pos.x > (ws.x) || pos.y < (100) || pos.y > (ws.y));
        };

        sf::Vector2f pos (100 + rand()%(ws.x), 100 + rand()%(ws.y));
        while(isInsidePlayerBounds(pos) || isOutsideWindowBounds(pos))
            pos = Vector2f(100 + rand()%(ws.x), 100 + rand()%(ws.y));

        enemies_.push_back(std::make_shared<Bugol>(pos.x, pos.y, &world));
        
        counter = 0;
    } else
        counter++;
}

void Enemies::handleOrbs() {
    auto it = xpOrbs_.begin();
    while(it != xpOrbs_.end()) {
        if(auto orb = *it) {
            orb->_animation->update(1);
            if(orb->getCollisionData()->colliding && 
			   orb->getCollisionData()->category == 
               (u32) CollidableType::XPFIELD) {
                orb->getCollisionData()->other_data->collided = true;

                it->reset();
                it = xpOrbs_.erase(it);
            } else  
                it++;
        }
    }
}

EnemyProperties Bugol::_bProperties(health, damage, damage_delay, agility, xp_range);
Bugol::Bugol(float x, float y, b2World* world) 
    : Enemy(x, y, world, new Box(8,8,1.f), "bugol.png") {

    _animation = new Animation(_sprite, _texture, 10, sf::Vector2u(0,0), 11, 10, size_);

    level = get_properties().level;

    _xp = get_properties().xp_range.first+(std::rand()%(int)get_properties().xp_range.second);
    _health = get_properties()._health;

    _data->damage_do = get_properties()._damage;
    _data->ddelay_do = _data->counter = get_properties().damage_delay;
}

void Bugol::level_up() {
    _bProperties.level++;

    _bProperties.xp_range.second += xp_range_buff;
    _bProperties._health += health_buff;
    _bProperties.damage_delay -= damage_delay_buff;
    _bProperties._damage += damage_buff;

}
