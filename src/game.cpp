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

tmx::Map loadMap() {
	tmx::Map map;
	map.load("assets/forest.tmx");
	return map;
}
Game::Game(Context& ctx):
	State(ctx, 1),
	map(loadMap()),
	avgFrame(0)
{
	sf::Vector2u ws = ctx.window.getSize();
	camera.setCenter({ ws.x * 0.5f, ws.y * 0.5f });
	camera.setSize(256.0f * ws.x / ws.y, 256.0f);
	testPlayer.setSize({ 32, 64 });
	for(auto& c: map.collisions()) ff.addObstacle(c->origin.x, c->origin.y, 32, 32);
	message = StateMessage::Push(std::make_unique<UserInterface>(ctx, this));
}

void Game::restartClock() {
	clock.restart();
}

void Game::tick() {
	auto elapsed = clock.getElapsedTime();
	clock.restart();
	avgFrame = avgFrame * 0.1 + elapsed.asSeconds() * 0.9;
	map.update(elapsed);

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

	sf::Vector2f ps = testPlayer.getSize();
	sf::Vector2f pp = testPlayer.getPosition() + speed;
	sf::Vector2f pc = pp + ps * 0.5f;
	testPlayer.setPosition(pp);

	sf::Vector2f cs = camera.getSize();
	sf::Vector2f cc = camera.getCenter();
	sf::Vector2f tol = (cs - ps) * 0.5f - sf::Vector2f(32, 32);
	cc.x = std::clamp(cc.x, pc.x - tol.x, pc.x + tol.x);
	cc.y = std::clamp(cc.y, pc.y - tol.y, pc.y + tol.y);
	camera.setCenter(cc);

	ff.calculate(pc.x, pc.y, ps.x, ps.y);
	for(auto& e: testEnemies) {
		auto es = e.getSize();
		auto ep = e.getPosition();
		auto s = ff.query(ep.x, ep.y, es.x, es.y);
		s *= 64 * elapsed.asSeconds();
		e.setPosition(ep + s);
	}
}

void Game::render() {
	renderer.clear();
	map.render(renderer);
	for(auto& e: testEnemies) renderer.insert(0, e);
	renderer.insert(0, testPlayer);

	ctx.window.setView(camera);
	ctx.window.draw(renderer);
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
		<< (1 / avgFrame) << " fps" << std::endl;
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
		sf::RectangleShape rect;
		rect.setPosition(pos);
		rect.setSize({32, 32});
		testEnemies.push_back(rect);
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
