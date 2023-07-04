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

Game::Game(Context& ctx):
	State(ctx, 1),
	map("assets/forest.tmx"),
	avgFrame(0),
	enemies_(300)
{
	sf::Vector2u ws = ctx.window.getSize();
	camera.setCenter({ ws.x * 0.5f, ws.y * 0.5f });
	camera.setSize(256.0f * ws.x / ws.y, 256.0f);
	
	player_ = std::make_unique<Player>(ws.x*0.3f,ws.y*0.3f, &ctx.world, new Box(10, 10, 100.f), "frog.png", WeaponType::GUN);
	
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
	player_->_attack(ctx.window);

	enemies_.spawnEnemy(ctx.window, ctx.world, camera, *player_);

	sf::Vector2f ps = player_->getSize_();
	sf::Vector2f pp = player_->getPosition_();
	sf::Vector2f pc = pp + ps * 0.5f;
	

	sf::Vector2f cs = camera.getSize();
	sf::Vector2f cc = camera.getCenter();
	sf::Vector2f tol = (cs - ps) * 0.5f - sf::Vector2f(32, 32);
	// cc.x = std::clamp(cc.x, pc.x - tol.x, pc.x + tol.x);
	// cc.y = std::clamp(cc.y, pc.y - tol.y, pc.y + tol.y);
	camera.setCenter(sf::Vector2f(pp.x, pp.y));

	sf::Clock ffClock;
	for(std::weak_ptr e: enemies_.enemies_) ff.addEnemy(e.lock()->getPosition_(), e.lock()->getSize_());
	ff.calculate(pc, ps);
	ffCalcTime = ffCalcTime * 0.9 + 0.1 * ffClock.getElapsedTime().asMilliseconds();

	for(std::weak_ptr e: enemies_.enemies_) {
		auto es = e.lock()->getSize_();
		auto ep = e.lock()->getPosition_();
		auto s = ff.query(ep, es);

		if(s != sf::Vector2f(0,0))
			e.lock()->_move(s, ctx.window);
		else
			e.lock()->_move(*player_, ctx.window);
	}

	enemies_.handleEnemies();
	enemies_.handleOrbs();
	player_->handlePlayer();
}

void Game::render() {
	renderer.clear();
	map.render(renderer);

	for(auto& orb: enemies_.xpOrbs_)
		orb->renderOrb(renderer);
	for(std::weak_ptr enemy: enemies_.enemies_) {
		renderer.insert(0, enemy.lock()->get_drawable());
		enemy.lock()->getGUI().renderGUI(renderer);
	}
	
	for(std::weak_ptr projectile: player_->get_weapon()->get_cartridge()) 	
		renderer.insert(0, projectile.lock()->get_drawable());

	
	renderer.insert(0, player_->get_drawable());
	player_->getGUI().renderGUI(renderer);
	
	
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
