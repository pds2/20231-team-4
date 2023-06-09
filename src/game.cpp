#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Map.hpp>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <sstream>
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
	message = StateMessage::Push(std::make_unique<UserInterface>(ctx, this));
}

void Game::restartClock() {
	clock.restart();
}

Object::Object(CollisionObject collision):
	collision(collision)
{
	rectangle.setSize({50, 50});
}

void Game::tick() {
	auto elapsed = clock.getElapsedTime();
	clock.restart();
	avgFrame = avgFrame * 0.1 + elapsed.asSeconds() * 0.9;
	cworld.step(elapsed.asSeconds());
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
	if(speed.x || speed.y) {
		f32 t = 128 * elapsed.asSeconds() / hypot(speed.x, speed.y);
		speed.x *= t, speed.y *= t;
	}

	sf::Vector2f ppos = testPlayer.getPosition() + speed;
	sf::Vector2f pcenter = ppos + testPlayer.getSize() * 0.5f;
	testPlayer.setPosition(ppos);

	sf::Vector2f csize = camera.getSize();
	sf::Vector2f ccenter = camera.getCenter();
	ccenter.x = std::clamp(ccenter.x, pcenter.x - csize.x * 0.2f, pcenter.x + csize.x * 0.2f);
	ccenter.y = std::clamp(ccenter.y, pcenter.y - csize.y * 0.2f, pcenter.y + csize.y * 0.2f);
	camera.setCenter(ccenter);
}

void Game::render() {
	renderer.clear();
	map.render(renderer);
	renderer.insert(0, testPlayer);

	ctx.window.setView(camera);
	ctx.window.draw(renderer);

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

	for(auto& o: objects) ctx.window.draw(o.rectangle);

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
		camera.setCenter(pos);
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
