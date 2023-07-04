#include "player.hpp"

using namespace sf;
using namespace std;

void Player::default_config(WeaponType& weaponType) {
    switch(weaponType) {
        case WeaponType::GUN:
            _weapon = new Gun(weaponProperties(5,20,10));
            break;
    }
}

Player::Player(float x, float y, b2World* world, Shapeb2* shape, string texture, WeaponType weaponType) 
    : Collidable(x, y, world, shape, b2_dynamicBody, texture, _categoryBits, _maskBits), _pProperties(100, 10, 3), _gui(*this), xp_field(*this) {
    default_config(weaponType);
}
Player::Player(float x, float y, b2World* world, Shapeb2* shape, Color color, WeaponType weaponType) 
    : Collidable(x, y, world, shape, b2_dynamicBody, color, _categoryBits, _maskBits), _pProperties(100, 10, 3), _gui(*this), xp_field(*this) {
    default_config(weaponType);
}


Player::~Player() {
    delete _weapon;
}


/*
 * Player movement configurations
 */
void Player::_move(RenderWindow &window, View& camera) {
    /*
     * Vertical and Horizontal movements
     */
    
    auto isPressed = sf::Keyboard::isKeyPressed;
    using Key = sf::Keyboard::Key;
    if(isPressed(Key::Up) || isPressed(Key::W))
		velocity.y = -_pProperties._agility;
	else if(isPressed(Key::Down) || isPressed(Key::S))
		velocity.y = _pProperties._agility;
    else 
        velocity.y = 0;
	if(isPressed(Key::Left) || isPressed(Key::A))
		velocity.x = -_pProperties._agility;
	else if(isPressed(Key::Right) || isPressed(Key::D))
		velocity.x = _pProperties._agility;
    else
        velocity.x = 0;
	

    _body->SetLinearVelocity(velocity);
    
    /*
     * Rotate player according to mouse position
     */
    window.setView(camera);
    Vector2i mp = Mouse::getPosition(window);
    Vector2f mousePosition = window.mapPixelToCoords(mp);
    Vector2f targetVector = Vector2f(mousePosition.x, mousePosition.y) - position_;
    long double angleRadians = atan2(targetVector.x, -targetVector.y);
    _body->SetTransform(_body->GetPosition(), -angleRadians);

    updateMovement(window);
    _gui.updateHPBar();
    _gui.updateXPBar();
    xp_field.updateField();
    xp_field.handleField();
}

/*
 * Player attack configurations
 * Different types of projectiles are fired according to certain
 * requirements
 */
void Player::handleAttack(RenderWindow& window) {
    auto it = _weapon->get_cartridge().begin();
    while(it != _weapon->get_cartridge().end()) {
        if(auto proj = *it) {
            proj->get_body()->SetLinearVelocity(proj->get_velocity());
            proj->updateMovement(window);

            if(proj->getCollisionData()->colliding || 
               proj->get_body()->GetLinearVelocity() == b2Vec2(0,0) ||
               proj->_distance() >= proj->get_range()) {
                it->reset();
                it = _weapon->get_cartridge().erase(it);
            } else 
                it++;
        } else
            it = _weapon->get_cartridge().erase(it);
    }
}

void Player::_attack(sf::RenderWindow &window) {
    if(Keyboard::isKeyPressed(Keyboard::Space))
        _weapon->fire(ProjectileType::NORMAL, *this);
    handleAttack(window);
}

PlayerGUI::PlayerGUI(const Player& player) : player_(player) {
    this->initHPBar();
    this->initXPBar();
}

void PlayerGUI::initHPBar() {
    sf::Vector2f size(20.f, 1.f);
    this->hpBarBack.setSize(size);
    this->hpBarBack.setFillColor(sf::Color(250,50,50, 200));
    this->hpBarBack.setOrigin(size/2.f);
    this->hpBarBack.setPosition(player_.getPosition_().x, 
        (player_.getPosition_().y + player_.getSize_().y));

    this->hpBarInner.setSize(size);
    this->hpBarInner.setFillColor(sf::Color(20,250,20, 200));
    this->hpBarInner.setOrigin(size/2.f);
    this->hpBarInner.setPosition(this->hpBarBack.getPosition());
}

void PlayerGUI::updateHPBar() {
    sf::Vector2f size(20.f*(player_.get_properties()._health/player_.get_properties().get_default_health()), 1.f);

    this->hpBarBack.setPosition(player_.getPosition_().x, 
        (player_.getPosition_().y + player_.getSize_().y));
    this->hpBarInner.setPosition(this->hpBarBack.getPosition());
    this->hpBarInner.setSize(size);
}

void PlayerGUI::initXPBar() {
    sf::Vector2f size(20.f, 1.f);
    this->xpBarBack.setSize(size);
    this->xpBarBack.setFillColor(sf::Color(250,250,250,200));
    this->xpBarBack.setOrigin(size/2.f);
    this->xpBarBack.setPosition(this->hpBarBack.getPosition().x, 
        this->xpBarBack.getPosition().y + this->hpBarBack.getSize().y);

    this->xpBarInner.setSize(sf::Vector2f(0,0));
    this->xpBarInner.setFillColor(sf::Color(23,66,255,255));
    this->xpBarInner.setOrigin(size/2.f);
    this->xpBarInner.setPosition(this->xpBarBack.getPosition());
}

void PlayerGUI::updateXPBar() {
    sf::Vector2f size(20.f*(player_.get_properties()._xp/player_.get_properties().level_up), 1.f);

    this->xpBarBack.setPosition(player_.getPosition_().x, 
        (player_.getPosition_().y + player_.getSize_().y + 1.5));
    this->xpBarInner.setPosition(this->xpBarBack.getPosition());
    this->xpBarInner.setSize(size);
}

void PlayerGUI::renderGUI(ZRenderer& renderer) {
    renderer.insert(0, this->hpBarBack);
    renderer.insert(0, this->hpBarInner);
    renderer.insert(0, this->xpBarBack);
    renderer.insert(0, this->xpBarInner);
}

XpField::XpField(Player& p) : Collidable(p.getPosition_().x, p.getPosition_().y,
p.get_world(), new Circle(10.f, 1.f), b2_dynamicBody,
_categoryBits, _maskBits), player_(p) {}

void XpField::updateField() {
    _body->SetLinearVelocity(player_.get_velocity());
    if(_body->GetPosition() != player_.get_body()->GetPosition()) 
        _body->SetTransform(player_.get_body()->GetPosition(), 0);
}

void XpField::handleField() {
    player_.get_properties().update_xp(getCollisionData()->damage_take);
    getCollisionData()->damage_take = 0;
}