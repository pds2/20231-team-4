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
	map("assets/cave.tmx"),
	avgFrame(0),
	world({0, 0})
{
	world.SetContactListener(&contactListener);

	sf::Vector2u ws = ctx.window.getSize();
	camera.setCenter({ ws.x * 0.5f, ws.y * 0.5f });
	camera.setSize(256.0f * ws.x / ws.y, 256.0f);

	player = std::make_unique<Player>(
		&world,
		sf::Vector2f(100, 100),
		sf::Vector2f(16, 16),
		b2_dynamicBody,
		"assets/frog.png",
		PlayerProperties(100, 10, 3),
		WeaponType::GUN
	);

	for(auto& c: map.collisions()) {
		obstacles.push_back(std::make_unique<Collidable>(
			&world,
			sf::Vector2f(c.left, c.top),
			sf::Vector2f(c.width, c.height),
			b2_staticBody
		));
		ff.addObstacle<f32>({c.left, c.top}, {c.width, c.height});
	}
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

	ctx.window.setView(camera);
	auto mp = sf::Mouse::getPosition(ctx.window);
	sf::Vector2f mpos = ctx.window.mapPixelToCoords(mp);
	player->move();
	player->attack(mpos);

	sf::Vector2f ps = player->getSize();
	sf::Vector2f pp = player->getPosition();
	sf::Vector2f pc = pp + ps * 0.5f;

	sf::Vector2f cs = camera.getSize();
	sf::Vector2f cc = camera.getCenter();
	sf::Vector2f tol = (cs - ps) * 0.5f - sf::Vector2f(32, 32);
	cc.x = std::clamp(cc.x, pc.x - tol.x, pc.x + tol.x);
	cc.y = std::clamp(cc.y, pc.y - tol.y, pc.y + tol.y);
	camera.setCenter(cc);

	for(std::weak_ptr e: enemies.enemies) ff.addEnemy(e.lock()->getPosition(), e.lock()->getSize());
	ff.calculate(pc, ps);

	for(std::weak_ptr enemy: enemies.enemies) {
		auto e = enemy.lock();
		auto es = e->getSize();
		auto ep = e->getPosition();
		auto s = ff.query(ep, es);
		e->move(s);
	}

	world.Step(elapsed.asSeconds(), 6, 3);
	player->updatePosition();
	for(auto& e: enemies.enemies) e->updatePosition();
	player->updateProjectiles();
	enemies.updateHealths();
	player->updateHealth();
}

void Game::render() {
	renderer.clear();
	map.render(renderer);

	for(std::weak_ptr e: enemies.enemies)
		renderer.insert(0, *e.lock());
	
	for(std::weak_ptr projectile: player->get_weapon()->get_cartridge()) 	
		renderer.insert(0, *projectile.lock());

	renderer.insert(0, *player);

	ctx.window.setView(camera);
	ctx.window.draw(renderer);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		ctx.window.draw(ff);

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

		auto enemy = std::make_shared<Enemy>(
			&world,
			pos,
			sf::Vector2f(8, 8),
			b2_dynamicBody,
			Assets::bugol,
			EnemyProperties(10,10,10, 1)
		);
		enemies.enemies.push_back(enemy);
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
