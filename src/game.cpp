#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Map.hpp>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "game.hpp"
#include "menu.hpp"
#include "assets.hpp"


void updateMovement(Collidable &obj, sf::RenderWindow &window) {
	sf::Vector2f new_position (obj.get_body()->GetPosition().x*PPM, 
							   obj.get_body()->GetPosition().y*PPM);
	double rotation = -1*obj.get_body()->GetAngle() * DEG_PER_RAD;

	obj.setPosition_(new_position, rotation);
}

void handleAttack(std::vector<std::shared_ptr<Projectile>>& cartridge, sf::RenderWindow& window) {
	auto it = cartridge.begin();
	while(it != cartridge.end()) {
		if(auto proj = *it) {
			proj->get_body()->SetLinearVelocity(proj->get_velocity());
			updateMovement(*proj, window);

			if(proj->getCollisionData()->colliding || proj->get_body()->GetLinearVelocity() == b2Vec2(0,0) || proj->_distance() >= proj->get_range()) {
				it->reset();
				it = cartridge.erase(it);
			} else
				it++;
		} else {
			it = cartridge.erase(it);
		}
	}
}

void handleEnemies(std::vector<std::shared_ptr<Enemy>>& enemies, sf::RenderWindow& window) {
	auto it = enemies.begin();
	while(it != enemies.end()) {
		if(auto enemy = *it) {
			if(enemy->getCollisionData()->colliding && 
			   enemy->getCollisionData()->category == ((u32) CollidableType::PROJECTILE|(u32)CollidableType::DYNAMIC)) {

				enemy->get_properties()._health -= enemy->getCollisionData()->damage_take;
				enemy->getCollisionData()->colliding = 0;

				if(enemy->get_properties()._health <= 0) {
					it->reset();
					it = enemies.erase(it);
				} else 
					it++;
			} else	
				it++;
		} else
			it = enemies.erase(it);
	}
}


Game::Game(Context& ctx):
	State(ctx, 1),
	map("assets/forest.tmx"),
	avgFrame(0)
{
	sf::Vector2u ws = ctx.window.getSize();
	camera.setCenter({ ws.x * 0.5f, ws.y * 0.5f });
	camera.setSize(256.0f * ws.x / ws.y, 256.0f);
	
	player_ = std::make_unique<Player>(0,0,
									  &ctx.world,
									  new Box(10,10,100.f),
									  b2_dynamicBody,
									  "frog.png",
									  PlayerProperties(100, 10, 3),
									  WeaponType::GUN);
	//testPlayer.setSize({ 32, 64 });
	
	
	for(auto& c: map.collisions()) ff.addObstacle<f32>({c.left, c.top}, {c.width, c.height});
	message = StateMessage::Push(std::make_unique<UserInterface>(ctx, this));
}

void Game::restartClock() {
	clock.restart();
}

f32 ffCalcTime = 0;
void Game::tick() {
	auto elapsed = clock.getElapsedTime();
	clock.restart();
	avgFrame = avgFrame * 0.9 + elapsed.asSeconds() * 0.1;
	map.update(elapsed);

	player_->_move(ctx.window, camera);
	player_->_attack();
	/*
	sf::Vector2f speed;
	auto isPressed = sf::Keyboard::isKeyPressed;
	using Key = sf::Keyboard::Key;
	if(isPressed(Key::Up) || isPressed(Key::W))
		speed.y -= 1;
	if(isPressed(Key::Down) || isPressed(Key::S))
		speed.y += 1;
	if(isPressed(Key::Left) || isPressed(Key::A))
		speed.x -= 1;
	if(isPressed(Key::Right) || isPressed(Key::D))
		speed.x += 1;
	if(speed.x || speed.y) speed *= 128 * elapsed.asSeconds() / hypotf(speed.x, speed.y);
	*/
	updateMovement(*player_, ctx.window);
	handleAttack(player_->get_weapon()->get_cartridge(), ctx.window);
	



	sf::Vector2f ps = player_->getSize_();
	sf::Vector2f pp = player_->getPosition_(); //+ speed;
	sf::Vector2f pc = pp + ps * 0.5f;
	//testPlayer.setPosition(pp);

	sf::Vector2f cs = camera.getSize();
	sf::Vector2f cc = camera.getCenter();
	sf::Vector2f tol = (cs - ps) * 0.5f - sf::Vector2f(32, 32);
	cc.x = std::clamp(cc.x, pc.x - tol.x, pc.x + tol.x);
	cc.y = std::clamp(cc.y, pc.y - tol.y, pc.y + tol.y);
	camera.setCenter(cc);

	sf::Clock ffClock;
	for(std::weak_ptr e: enemies_.enemies_) ff.addEnemy(e.lock()->getPosition_(), e.lock()->getSize_());
	ff.calculate(pc, ps);
	ffCalcTime = ffCalcTime * 0.9 + 0.1 * ffClock.getElapsedTime().asMilliseconds();

	for(std::weak_ptr e: enemies_.enemies_) {
		auto es = e.lock()->getSize_();
		auto ep = e.lock()->getPosition_();
		auto s_ = ff.query(ep, es);
		sf::Vector2f s;
		if(s_.x || s_.y)
			s = s_;
		//s *= 64 * elapsed.asSeconds();
		//e.setPosition(ep + s);
		
		
		e.lock()->_move(s);
		updateMovement(*e.lock(), ctx.window);
	}

	handleEnemies(enemies_.enemies_, ctx.window);
}

void Game::render() {
	renderer.clear();
	map.render(renderer);

	for(std::weak_ptr e: enemies_.enemies_) 
		renderer.insert(0, e.lock()->get_sprite());
	
	for(std::weak_ptr projectile: player_->get_weapon()->get_cartridge()) 	
		renderer.insert(0, *projectile.lock()->get_sfml_shape());

	//renderer.insert(0, testPlayer);
	renderer.insert(0, player_->get_sprite());


	ctx.window.setView(camera);
	ctx.window.draw(renderer);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		ctx.window.draw(ff);

	ctx.window.setView(camera);
	auto mp = sf::Mouse::getPosition(ctx.window);
	sf::Vector2f pos = ctx.window.mapPixelToCoords(mp);
	sf::Text debug;
	debug.setPosition(20, 20);
	debug.setFont(Assets::font);

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2)
		<< pos.x << ' ' << pos.y << std::endl
		<< (1 / avgFrame) << " fps" << std::endl
		<< ffCalcTime << "ms ff calc" << std::endl;
	debug.setString(ss.str());

	sf::View v;
	v.setViewport({{0, 0}, {1, 1}});
	ctx.window.setView(v);
	ctx.window.draw(debug);
}
void Game::handleEvent(sf::Event event) {
	if(event.type == sf::Event::MouseWheelScrolled) {
		auto cs = camera.getSize();
		float perc = event.mouseWheelScroll.delta > 0 ? 0.9 : 1.1;
		cs.x *= perc, cs.y *= perc;
		camera.setSize(cs);
	}
	if(event.type == sf::Event::MouseButtonPressed) {
		ctx.window.setView(camera);
		auto mp = sf::Mouse::getPosition(ctx.window);
		auto pos = ctx.window.mapPixelToCoords(mp);
		
		/*
		sf::RectangleShape rect;
		rect.setPosition(pos);
		rect.setSize({32, 32});
		testEnemies.push_back(rect);
		*/
		enemies_.enemies_.push_back(std::make_shared<Enemy>(
									pos.x, pos.y, 
									&ctx.world, 
									new Box(8, 8, 1.f), 
									b2_dynamicBody, "bugol.png", 
									EnemyProperties(10,10,10, 1)));
	}
	if(event.type == sf::Event::Resized) {
		sf::Vector2u ws = ctx.window.getSize();
		camera.setSize(ws.x, ws.y);
	}
}
Game::~Game() {}

UserInterface::UserInterface(Context& ctx, Game* game):
	game(game),
	State(ctx, 0) {}
void UserInterface::tick() {
	game->tick();
}
void UserInterface::render() {}
void UserInterface::handleEvent(sf::Event event) {
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		message = StateMessage::Into(std::make_unique<PauseMenu>(ctx, game));
	game->handleEvent(event);
}
UserInterface::~UserInterface() {}
